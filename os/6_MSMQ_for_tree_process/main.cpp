#include <utility>
#include "msmq_lib.cpp"

/*
 * N a1 a2 ... an -> SIGMA(ai)
 * absulute balanced tree (avl)
 * TODO: 0
 */

class TMainProcess {
public:
    TMainProcess(std::wstring main_port_name, std::wstring root_vertex_id) {
        InitClassFields(std::move(main_port_name), std::move(root_vertex_id));
    }

    void CreateTree(std::wstring& id) {
        CreateNode(id);
    }

    void SendTreeMessage(std::vector <std::wstring>& message) {
        SendTreeMessageImpl(message);
    }

    void PingById(std::wstring id) {
        PingByIdImpl(id);
    }

    bool IsCreated() {
        return PingRoot() && root_id_;
    }

    ~TMainProcess() {
        DestroyTree();
    }

private:
    HANDLE mutex_{};
    HANDLE send_mutex_{};
    std::wstring msmq_port_prefix_ = L".\\PRIVATE$\\";
    std::wstring main_name_suffix_ = L"";
    std::wstring main_port_name_ = L"";
    std::wstring root_vertex_id_ = L"";
    std::wstring root_vertex_id_suffix_ = L"";
    std::wstring w_message_ = L"";
    const wchar_t* message_ = nullptr;
    const wchar_t* root_id_ = nullptr;
    const wchar_t* main_name_ = nullptr;

    void InitClassFields(std::wstring in_main_port_name, std::wstring in_root_vertex_id) {
        main_port_name_ = std::move(in_main_port_name);
        root_vertex_id_ = std::move(in_root_vertex_id);
        main_name_suffix_ = main_port_name_;
        main_port_name_ = (msmq_port_prefix_ + main_port_name_);
        main_name_ = main_port_name_.c_str();
        if (root_vertex_id_ != L"nil") {
            root_vertex_id_suffix_ = root_vertex_id_;
            root_vertex_id_ = (msmq_port_prefix_ + root_vertex_id_);
            root_id_ = root_vertex_id_.c_str();
        }
        MSMQ_CreateQueue(main_name_);
        mutex_ = CreateMutex(nullptr, FALSE, "mutex");
        send_mutex_ = CreateMutex(nullptr, TRUE, "send_mutex");
    }

    void SendTreeMessageImpl(std::vector <std::wstring>& message) {
        if (!root_id_) {
            std::cout << "Error: Node not exist" << std::endl;
            return;
        } else if (!PingRoot()) {
            std::cout << "Error: Node is unavailable" << std::endl;
            return;
        }

        if (message[0] == L"create" || message[0] == L"remove") {
            SendShortMessage(message);
            if (root_vertex_id_suffix_ == message[1] && message[0] == L"remove") {
                root_id_ = nullptr;
            }
        } else {
            SendLongMessage(message);
        }
    }

    void SendShortMessage(std::vector <std::wstring>& message) {
        if (!PingRoot()) {
            std::cout << "Error: Node is unavailable" << std::endl;
            return;
        }

        w_message_ = message[0] + L" " + message[1];
        message_ = w_message_.c_str();
        MSMQ_SendMessage(root_id_, message_);
    }

    void SendLongMessage(std::vector <std::wstring>& message) {
        if (!PingRoot()) {
            std::cout << "Error: Node is unavailable" << std::endl;
            return;
        }

        message.emplace_back(L"end");
    //    WaitForSingleObject(send_mutex_, 0);
     //   std::cout << "lock" << std::endl;

        std::wstring pref_mes = message[0] + L" " + message[1];
        const int w_size = message.size();
        MSMQ_SendMessage(root_id_, pref_mes.c_str());

        for (int i = 2; i < w_size; i += 10) {
            std::wstring ws = L"";
            for (int j = i; j < w_size && j - i < 10; ++j) {
                ws += message[j];
                ws += L" ";
            }
            Sleep(3);
            MSMQ_SendMessage(root_id_, ws.c_str());
        }
    }

    void CreateNode(const std::wstring& id) {
        PROCESS_INFORMATION p_info;
        STARTUPINFO s_info;
        ZeroMemory(&s_info, sizeof(s_info));
        s_info.cb = sizeof s_info;

        std::wstring w_args = (main_name_suffix_ + L" " + id);
        std::string s_args(w_args.begin(), w_args.end());

        char* args = new char[s_args.length() + 1];
        strcpy(args, s_args.c_str());

        if (CreateProcess(R"(C:\Users\Admin\CLionProjects\msmq_lab\cmake-build-debug\msmq_node.exe)", args,
                        nullptr, nullptr, FALSE, 0, nullptr, nullptr, &s_info, &p_info)) {
            // here save data
            root_vertex_id_suffix_ = id;
            root_vertex_id_ = msmq_port_prefix_ + id;
            root_id_ = root_vertex_id_.c_str();
            //end
        } else {
            WaitForSingleObject(mutex_, 0);
            DWORD error = GetLastError();
            std::cout << "Error: Can't create node (" << error << ")" << std::endl;
            ReleaseMutex(mutex_);
        }
        delete[] args;
    }

    bool PingRoot() {
        std::wstring ping_message = (L"ping " + main_name_suffix_);
        MSMQ_SendMessage(root_id_, ping_message.c_str());
        Sleep(15);
        wchar_t* back = nullptr;
        MSMQ_ReadMessage(main_name_, &back);
        if (!back) {
            return false;
        } else {
            delete[] back;
            return true;
        }
    }

    void PingByIdImpl(std::wstring& ws) {
        std::wstring ping_message = L"ping " + main_name_suffix_;
        std::wstring ping_port = msmq_port_prefix_ + ws;

        wchar_t* clear = nullptr;
        MSMQ_SendMessage(main_name_, clear);
        MSMQ_ReadMessage(main_name_, &clear);

        MSMQ_SendMessage(ping_port.c_str(), ping_message.c_str());
        Sleep(15);

        wchar_t* back = nullptr;
        MSMQ_ReadMessage(main_name_, &back);
        std::wcout << back << L" by png " << main_name_ << std::endl;
        if (!back) {
            HANDLE mutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "mutex");
            WaitForSingleObject(mutex, 0);
            std::cout << "Ok: 0" << std::endl;
            ReleaseMutex(mutex);
        } else {
            delete[] back;
            HANDLE mutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "mutex");
            WaitForSingleObject(mutex, 0);
            std::cout << "Ok: 1" << std::endl;
            ReleaseMutex(mutex);
        }
    }

    void DestroyTree() {
        if (PingRoot()) {
            MSMQ_SendMessage(root_id_, L"destroy");
        }
    }
};


int main() {
    std::wstring main_port_name;
    std::wstring root_vertex_id;
    std::cout << "Please enter new port name:";
    std::wcin >> main_port_name;
    std::cout << "Please enter tree root id if exist ('nil' else):";
    std::wcin >> root_vertex_id;

    std::unique_ptr <TMainProcess> main_process = std::make_unique<TMainProcess>(main_port_name, root_vertex_id);

    while (true) {
        std::wstring cmd;
        std::wcin >> cmd;

        if (cmd == L"break") {
            break;
        }

        if (cmd == L"create") {
            std::wstring id;
            std::wstring parent;
            std::wcin >> id >> parent;

            if (main_process -> IsCreated()) {
                std::vector <std::wstring> message(2);
                message[0] = cmd;
                message[1] = id;
                main_process -> SendTreeMessage(message);
            } else {
                main_process -> CreateTree(id);
            }
            continue;
        }

        if (cmd == L"remove") {
            std::wstring id;
            std::wcin >> id;
            std::vector <std::wstring> message(2);
            message[0] = L"remove";
            message[1] = id;

            main_process -> SendTreeMessage(message);
            continue;
        }

        if (cmd == L"exec") {
            std::wstring id;
            std::wcin >> id;
            std::vector <std::wstring> message(2);
            message[0] = L"exec";
            message[1] = id;

            int total_nums;
            std::cin >> total_nums;
            message.push_back(std::to_wstring(total_nums));

            for (int i = 0; i < total_nums; ++i) {
                std::wstring num;
                std::wcin >> num;
                message.push_back(num);
            }
            main_process -> SendTreeMessage(message);
            continue;
        }

        if (cmd == L"ping") {
            std::wstring id;
            std::wcin >> id;

            main_process -> PingById(id);
            continue;
        }


        HANDLE print_mutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "mutex");
        WaitForSingleObject(print_mutex, 0);
        std::cout << "Error: Unexpected command" << std::endl;
        ReleaseMutex(print_mutex);
    }

    return 0;
}
