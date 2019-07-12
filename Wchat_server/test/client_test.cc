#include <ctime>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>

#include "msg_fragment.h"
#include "server_json.h"

#define SERVER_ADDR "server-addr"
#define SERVER_PORT "server-port"
#define THREAD_NUM  1000
#define MSG_SEND_NUM 1000
struct sock {
    int num;
    int sock_fd;
    struct fragment frag;
};

bool stop = false;

void print_frag(const struct fragment& frag);
void received_data_function(struct fragment& frag);
bool frag_compare(const struct fragment& frag1, const struct fragment& frag2);
void create_sockets(struct sock sock[], int size, int port, std::string ser_addr);
void* thread_func(void* arg);

int* buf = nullptr;
void init_buffer(void);
void insert_data(int x, int y, int value);
void print_msg(void);

int main(void)
{
    short port;
    std::string ser_addr;
    Json& json_instance = Json::get_json_instance("../../server_config/server.json");

    json_instance.read_value(SERVER_ADDR, ser_addr);
    json_instance.read_value(SERVER_PORT, port);

    struct fragment frag;
    received_data_function(frag);
    init_buffer();

    struct sock sock_arg[THREAD_NUM];
    pthread_t thread_id[THREAD_NUM];

    create_sockets(sock_arg, THREAD_NUM, port, ser_addr);
    for (int i = 0; i < THREAD_NUM; ++i) {
        sock_arg[i].frag = frag;
        pthread_create(&thread_id[i], NULL, thread_func, (void*)&sock_arg[i]);
    }

    for (int i = 0; i < THREAD_NUM; ++i) {
        pthread_join(thread_id[i], NULL);
    }

    for (int i = 0; i < THREAD_NUM; ++i) {
        close(sock_arg[i].sock_fd);
    }
    print_msg();
    return 0;
}

void create_sockets(struct sock sock[], int size, int port, std::string ser_addr)
{
    for (int i = 0; i < size; ++i) {
        sock[i].num = i;
        sock[i].sock_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (sock[i].sock_fd == -1) {
            perror("socket");
            exit(EXIT_FAILURE);
        }

        struct sockaddr_in cliaddr;
        cliaddr.sin_family = AF_INET;
        cliaddr.sin_port = htons(port);
        inet_pton(AF_INET, ser_addr.c_str(), &cliaddr.sin_addr);

        socklen_t addrlen = sizeof(cliaddr);
        if (connect(sock[i].sock_fd,  (struct sockaddr *)&cliaddr, addrlen) != 0) {
            perror("connect");
        }
    }
}

void* thread_func(void* arg)
{
    struct fragment recv_frag;
    struct sock* sock_stru = (struct sock*)arg;

    std::cout << "thread: " << sock_stru->num << " is running." << std::endl;
    clock_t start, end;
    for (int i = 0; i < MSG_SEND_NUM; ++i) {
        write(sock_stru->sock_fd, (void*)&sock_stru->frag, sizeof(struct fragment));
        start = clock();
        read(sock_stru->sock_fd, (void*)&recv_frag, sizeof(struct fragment));
        end = clock();
        insert_data(sock_stru->num, i, (int)(end-start));
        memset(&recv_frag, 0, sizeof(recv_frag));
    }
    close(sock_stru->sock_fd);
}

void received_data_function(struct fragment& frag)
{
    std::string tmp_str;

    std::cout << "start received_data_function" << std::endl;
    std::fstream frag_file("../../server_config/tmp_fragment");
    if (!frag_file) {
        std::cerr << "open ../../server_config/tmp_fragment failed." << std::endl;
        return ;
    }
    frag_file >> tmp_str;
    frag.msg_type = std::stoi(tmp_str); 
    frag_file >> tmp_str;
    sprintf(frag.sender_account, "%s", tmp_str.c_str());
    frag_file >> tmp_str;
    sprintf(frag.receiver_account, "%s", tmp_str.c_str());
    frag_file >> tmp_str;
    sprintf(frag.group_account, "%s", tmp_str.c_str());
    frag_file >> frag.fragment_amount;
    frag.fragment_amount -= '0'; 
    frag_file >> frag.fragment_position;
    frag.fragment_position -= '0';
    frag_file >> tmp_str;
    for (int i = 0; i < tmp_str.size(); ++i) {
        if (tmp_str[i] == '+') {
            tmp_str[i] = ' ';
            break;
        }
    }
    sprintf(frag.data, "%s", tmp_str.c_str());
    frag.fragment_length = tmp_str.size();
    std::cout << "str_size: " << tmp_str.size() << std::endl;
    std::cout << "frag_size: " << (int)frag.fragment_length << std::endl;
    print_frag(frag);
}

void print_frag(const struct fragment& frag)
{
    std::cout << "msg_type: " << (int)frag.msg_type << std::endl;
    std::cout << "sender_account: " << frag.sender_account << std::endl;
    std::cout << "receiver_account: " <<  frag.receiver_account << std::endl;
    std::cout << "group_account: " << frag.group_account << std::endl;
    std::cout << "fragment_amount: " << (int)frag.fragment_amount << std::endl;
    std::cout << "fragment_position: " << (int)frag.fragment_position << std::endl;
    std::cout << "fragment_length: " << (int)frag.fragment_length << std::endl;
    std::cout << "fragment_identity: " << frag.identity << std::endl;
    std::cout << "fragment_data: " << frag.data << std::endl;
}

bool frag_compare(const struct fragment& frag1, const struct fragment& frag2)
{
    if (frag1.msg_type != frag2.msg_type) {
        return false;
    }
    if (strncmp(frag1.sender_account, frag2.sender_account, strlen(frag1.sender_account)) != 0) {
        return false;
    }
    if (strncmp(frag1.receiver_account, frag2.receiver_account, strlen(frag1.receiver_account)) != 0) {
        return false;
    }
    if (strncmp(frag1.group_account, frag2.group_account, strlen(frag1.group_account)) != 0) {
        return false;
    }
    if (frag1.fragment_amount != frag2.fragment_amount) {
        return false;
    }
    if (frag1.fragment_position != frag2.fragment_position) {
        return false;
    }
    if (frag1.fragment_length != frag2.fragment_length) {
        return false;
    }
    if (strncmp(frag1.data, frag2.data, strlen(frag1.data)) != 0) {
        return false;
    }
    return true;
}

void init_buffer(void)
{
    buf = (int*)malloc(THREAD_NUM * MSG_SEND_NUM * sizeof(int));
    if (buf == NULL) {
        std::cout << "init_buffer calls failed: malloc." << std::endl;
        return ;
    }
}
void insert_data(int x, int y, int value)
{
    if (x < 0 || x >= THREAD_NUM) {
        return ;
    }
    if (y < 0 || y >= MSG_SEND_NUM) {
        return ;
    }

    buf[THREAD_NUM * x + y] = value;
}
void print_msg(void)
{
    for (int i = 0; i < THREAD_NUM; ++i) {
        int sum  = 0;
        for (int j = 0; j < MSG_SEND_NUM; ++j) {
            sum += buf[i * THREAD_NUM + j];
        }
        std::cout << "thread_num: " << i << " avg: " << (double)sum  / MSG_SEND_NUM/CLOCKS_PER_SEC << " s" << std::endl;
    }
    free(buf);
}