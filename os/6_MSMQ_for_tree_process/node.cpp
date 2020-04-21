#include <chrono>
#include <thread>
#include "msmq_lib.cpp"

//    MSMQ_SendMessage(port_name, L"123 456 789 9999 kekw");
//    MSMQ_SendMessage(port_name, L"ProcessKill");

void Example() {
    const wchar_t *address = L".\\PRIVATE$\\MSMQ1";
    std::wcout << address << std::endl;
    HRESULT create = MSMQ_CreateQueue(address);
    HRESULT send = MSMQ_SendMessage(address, L"123123");
    wchar_t *mas = nullptr;
    HRESULT get = MSMQ_ReadMessage(address, &mas);
    std::wcout << mas << std::endl;
}

class TNode {
public:
    TNode(char* main_port_name, char* id) {
        Init(main_port_name, id);
    }

    const wchar_t* GetMSMQPort() {
        return this_node_msmq_id_;
    }

    void PerformEvents(wchar_t* message) {
        if (message[0] == L'p') {
            PingBack(message);
        }

        if (message[0] == L'c') {
            MakeCreateEventImpl(message);
        }

        if (message[0] == L'r') {
            MakeRemoveEventImpl(message);
        }

        if (message[0] == L'e') {
            MakeExecEventImpl(message);
        }

        if (message[0] == L'd') {
            is_active_ = false;
        }

        delete[] message;
    }

    bool IsActive() {
        return is_active_;
    }

    ~TNode() {
        SubTreeDestroy();
    }

private:
    std::mutex print_;
    bool is_active_ = true;
    int id_ = 0;
    const wchar_t* this_node_msmq_id_ = nullptr;
    const wchar_t* main_name_ = nullptr;
    const wchar_t* left_node_name_ = nullptr;
    const wchar_t* right_node_name_ = nullptr;
    std::wstring w_main_port_suf_ = L"";
    std::wstring w_main_port_ = L"";
    std::wstring w_this_msmq_port_id_ = L"";
    std::wstring msmq_port_prefix_ = L".\\PRIVATE$\\";
    std::wstring w_left_suffix_ = L"";
    std::wstring w_right_suffix_ = L"";
    std::wstring w_left_msmq_ = L"";
    std::wstring w_right_msmq_ = L"";

    void Init(char* main_port_name, char* id) {
        w_main_port_ = CharStarForWstring(main_port_name);
        w_main_port_suf_ = w_main_port_;
        w_this_msmq_port_id_ = CharStarForWstring(id);
        std::string s_id(id);
        w_main_port_ = (msmq_port_prefix_ + w_main_port_);
        w_this_msmq_port_id_ = (msmq_port_prefix_ + w_this_msmq_port_id_);

        main_name_ = w_main_port_.c_str();
        id_ = std::stoi(s_id);
        this_node_msmq_id_ = w_this_msmq_port_id_.c_str();

        MSMQ_CreateQueue(this_node_msmq_id_);
        int pid = GetCurrentProcessId();

        HANDLE mutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "mutex");
        WaitForSingleObject(mutex, 0);
        std::cout << "Ok: " << pid << " " << id_ << std::endl;
        ReleaseMutex(mutex);
    }

    void PingBack(wchar_t* message) {
        std::wistringstream wss(message);
        std::wstring inf_id;
        wss >> inf_id >> inf_id;
       // std::wstring w_id = std::to_wstring(id_);
        std::wstring send_back = (msmq_port_prefix_ + inf_id);

        MSMQ_SendMessage(send_back.c_str(), L"1");
    }

    bool Ping(const wchar_t* node) {
        std::wstring id = std::to_wstring(id_);
        std::wstring ping_message = (L"ping " + id);

        MSMQ_SendMessage(node, ping_message.c_str());
        Sleep(25);
        wchar_t* back = nullptr;
        MSMQ_ReadMessage(this_node_msmq_id_, &back);

        if (!back) {
            return false;
        } else {
            delete[] back;
            return true;
        }
    }

    void MakeCreateEventImpl(wchar_t* message) {
        std::wistringstream wss(message);
        std::wstring inf_id;
        std::wstring cmd;

        wss >> cmd >> inf_id;
        int i_id = std::stoi(inf_id);

        if (i_id == id_) {
            HANDLE mutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "mutex");
            WaitForSingleObject(mutex, 0);
            std::cout << "Error: Already exist" << std::endl;
            ReleaseMutex(mutex);
        } else if (i_id < id_) {
            std::wstring re_send = cmd + L" " + inf_id;
            if (left_node_name_) {
                if (Ping(left_node_name_)) {
                    MSMQ_SendMessage(left_node_name_, re_send.c_str());
                } else {
                    HANDLE mutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "mutex");
                    WaitForSingleObject(mutex, 0);
                    std::cout << "Error: Node is unavailable" << std::endl;
                    ReleaseMutex(mutex);
                }
            } else {
                std::wstring w_creating_args = (w_main_port_suf_ + L" " + inf_id);
                std::string s_args(w_creating_args.begin(), w_creating_args.end());

                PROCESS_INFORMATION p_info;
                STARTUPINFO s_info;
                ZeroMemory(&s_info, sizeof(s_info));
                s_info.cb = sizeof s_info;
                char* args = new char[s_args.length() + 1];
                strcpy(args, s_args.c_str());

                if (CreateProcess(R"(C:\Users\Admin\CLionProjects\msmq_lab\cmake-build-debug\msmq_node.exe)", args,
                                  nullptr, nullptr, FALSE, 0, nullptr, nullptr, &s_info, &p_info)) {
                    w_left_suffix_ = inf_id;
                    w_left_msmq_ = msmq_port_prefix_ + inf_id;
                    left_node_name_ = w_left_msmq_.c_str();
                } else {
                    HANDLE mutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "mutex");
                    WaitForSingleObject(mutex, 0);
                    DWORD error = GetLastError();
                    std::cout << "Error: Can't create node (" << error << ")" << std::endl;
                    ReleaseMutex(mutex);
                }
                delete[] args;
            }
        } else {
            std::wstring re_send = cmd + L" " + inf_id;

            if (right_node_name_) {
                if (Ping(right_node_name_)) {
                    MSMQ_SendMessage(right_node_name_, re_send.c_str());
                } else {
                    HANDLE mutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "mutex");
                    WaitForSingleObject(mutex, 0);
                    std::cout << "Error: Node is unavailable" << std::endl;
                    ReleaseMutex(mutex);
                }
            } else {
                std::wstring w_creating_args = (w_main_port_suf_ + L" " + inf_id);
                std::string s_args(w_creating_args.begin(), w_creating_args.end());

                PROCESS_INFORMATION p_info;
                STARTUPINFO s_info;
                ZeroMemory(&s_info, sizeof(s_info));
                s_info.cb = sizeof s_info;
                char *args = new char[s_args.length() + 1];
                strcpy(args, s_args.c_str());

                if (CreateProcess(R"(C:\Users\Admin\CLionProjects\msmq_lab\cmake-build-debug\msmq_node.exe)", args,
                                  nullptr, nullptr, FALSE, 0, nullptr, nullptr, &s_info, &p_info)) {
                    w_right_suffix_ = inf_id;
                    w_right_msmq_ = msmq_port_prefix_ + inf_id;
                    right_node_name_ = w_right_msmq_.c_str();
                } else {
                    HANDLE mutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "mutex");
                    WaitForSingleObject(mutex, 0);
                    DWORD error = GetLastError();
                    std::cout << "Error: Can't create node (" << error << ")" << std::endl;
                    ReleaseMutex(mutex);
                }
                delete[] args;
            }
        }
    }

    void MakeRemoveEventImpl(wchar_t* message) {
        std::wistringstream wss(message);
        std::wstring inf_id;
        std::wstring cmd;
        wss >> cmd >> inf_id;

        std::string s_id(inf_id.begin(), inf_id.end());
        int id = std::stoi(s_id);

        std::wstring re_send = cmd + L" " + inf_id;

        if (id == id_) {
            HANDLE mutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "mutex");
            WaitForSingleObject(mutex, 0);
            std::cout << "Ok" << std::endl;
            is_active_ = false;
            ReleaseMutex(mutex);
        } else if (id < id_) {
            if (!left_node_name_) {
                HANDLE print_mutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "mutex");
                WaitForSingleObject(print_mutex, 0);
                std::cout << "Error: Node not found" << std::endl;
                ReleaseMutex(print_mutex);
            } else{
                if (Ping(left_node_name_)) {
                    MSMQ_SendMessage(left_node_name_, re_send.c_str());
                    if (inf_id == w_left_suffix_) {
                        w_left_suffix_ = L"";
                        left_node_name_ = nullptr;
                        MSMQ_SendMessage(left_node_name_, L"destroy");
                    }
                } else {
                    HANDLE mutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "mutex");
                    WaitForSingleObject(mutex, 0);
                    std::cout << "Error: Node is unavailable" << std::endl;
                    ReleaseMutex(mutex);
                }
            }
        } else {
            if (!right_node_name_) {
                HANDLE print_mutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "mutex");
                WaitForSingleObject(print_mutex, 0);
                std::cout << "Error: Node not found" << std::endl;
                ReleaseMutex(print_mutex);
            } else{
                if (Ping(right_node_name_)) {
                    MSMQ_SendMessage(right_node_name_, re_send.c_str());
                    if (inf_id == w_right_suffix_) {
                        w_right_suffix_ = L"";
                        right_node_name_ = nullptr;
                        MSMQ_SendMessage(right_node_name_, L"destroy");
                    }
                } else {
                    HANDLE mutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "mutex");
                    WaitForSingleObject(mutex, 0);
                    std::cout << "Error: Node is unavailable" << std::endl;
                    ReleaseMutex(mutex);
                }
            }
        }
    }

    void MakeExecEventImpl(wchar_t* message) {
        std::vector <std::wstring> get_message;
        std::wstringstream wss(message);
        std::wstring ws;

        while (wss >> ws) {
            get_message.emplace_back(ws);
        }
        delete[] message;

        while (true) {
            wchar_t* mes = nullptr;
            bool ok = true;
            MSMQ_ReadMessage(this_node_msmq_id_, &mes);
            std::wstringstream wss_in(mes);
            while (wss_in >> ws) {
                get_message.emplace_back(ws);
                if (ws == L"end") {
                    ok = false;
                    break;
                }
            }
            delete[] mes;
            if (!ok) {
                break;
            }
        }

  //      HANDLE send_mutex = OpenMutexA(MUTEX_ALL_ACCESS, TRUE, "send_mutex");
  //      ReleaseMutex(send_mutex);
  //      std::cout << "unlock" << std::endl;

        std::string s_id(get_message[1].begin(), get_message[1].end());
        int id = std::stoi(s_id);

        std::string s_nums(get_message[2].begin(), get_message[2].end());
        int nums = std::stoi(s_nums);

        if (id == id_) {
            long long sum = 0;
            for (int i = 3; i < nums + 3; ++i) {
                std::string s(get_message[i].begin(), get_message[i].end());
                sum += std::stoll(s);
            }
            HANDLE print_mutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "mutex");
            WaitForSingleObject(print_mutex, 0);
            std::cout << "Ok:" << id_ << ": " << sum <<  std::endl;
            ReleaseMutex(print_mutex);
        } else {
            if (id < id_) {
                if (!left_node_name_) {
                    HANDLE print_mutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "mutex");
                    WaitForSingleObject(print_mutex, 0);
                    std::cout << "Error: Node not found" << std::endl;
                    ReleaseMutex(print_mutex);
                } else {
                    if (Ping(left_node_name_)) {
               //         send_mutex = OpenMutexA(MUTEX_ALL_ACCESS, TRUE, "send_mutex");
               //         WaitForSingleObject(send_mutex, 0);

                        std::wstring pref_mes = get_message[0] + L" " + get_message[1];
                        const int w_size = get_message.size();
                        MSMQ_SendMessage(left_node_name_, pref_mes.c_str());

                        for (int i = 2; i < w_size; i += 10) {
                            std::wstring ws = L"";
                            for (int j = i; j < w_size && j - i < 10; ++j) {
                                ws += get_message[j];
                                ws += L" ";
                            }
                            Sleep(1);
                            MSMQ_SendMessage(left_node_name_, ws.c_str());
                        }
                    } else {
                        HANDLE mutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "mutex");
                        WaitForSingleObject(mutex, 0);
                        std::cout << "Error: Node is unavailable" << std::endl;
                        ReleaseMutex(mutex);
                    }
                }
            } else {
                if (!right_node_name_) {
                    HANDLE print_mutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "mutex");
                    WaitForSingleObject(print_mutex, 0);
                    std::cout << "Error: Not found" << std::endl;
                    ReleaseMutex(print_mutex);
                } else {
                    if (Ping(right_node_name_)) {
                        //HANDLE send_mutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "send_mutex");
             //           WaitForSingleObject(send_mutex, 0);
            //            std::cout << "lock" << std::endl;

                        std::wstring pref_mes = get_message[0] + L" " + get_message[1];
                        const int w_size = get_message.size();
                        MSMQ_SendMessage(right_node_name_, pref_mes.c_str());


                        for (int i = 2; i < w_size; i += 10) {
                            std::wstring ws = L"";
                            for (int j = i; j < w_size && j - i < 10; ++j) {
                                ws += get_message[j];
                                ws += L" ";
                            }

                            MSMQ_SendMessage(right_node_name_, ws.c_str());
                            Sleep(3);
                        }

                    } else {
                        HANDLE mutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "mutex");
                        WaitForSingleObject(mutex, 0);
                        std::cout << "Error: Node is unavailable" << std::endl;
                        ReleaseMutex(mutex);
                    }
                }
            }
        }
    }

    void SubTreeDestroy() {
        wchar_t* clear = nullptr;
        MSMQ_SendMessage(this_node_msmq_id_, clear);
        MSMQ_ReadMessage(this_node_msmq_id_, &clear);

        if (left_node_name_) {
            MSMQ_SendMessage(left_node_name_,L"destroy");
        }

        if (right_node_name_) {
            MSMQ_SendMessage(right_node_name_, L"destroy");
        }
    }
};



int main(int argc, char *argv[]) {
    std::unique_ptr <TNode> node = std::make_unique<TNode>(argv[argc - 2], argv[argc - 1]);

    wchar_t* message = nullptr;

    while (true) {
        if (message != nullptr) {
            delete[] message;
            message = nullptr;
        }

        MSMQ_ReadMessage(node -> GetMSMQPort(), &message);

        if (message != nullptr) { // tut lock
            std::mutex mtx;

            mtx.lock();
            node -> PerformEvents(message);
            if (!node -> IsActive()) {
                break;
            }
            mtx.unlock();
        }
    }

    return 0;
}
