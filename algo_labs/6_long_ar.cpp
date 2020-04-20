#include <iostream>
#include <vector>
#include <cmath>

class TCalculator {
public:
    void PerformOperation(std::string &first_number, std::string &second_number, char operation) {
        OperationImpl(first_number, second_number, operation);
    }

    ~TCalculator() {
        ClearImpl();
    }

private:
    std::vector <long long> first_num_;
    std::vector <long long> second_num_;
    long long base_ = 10 * 10 * 10 * 10 * 10;
    int digits_in_numeral_sys_ = 5;
    int limb_base_ = 10;
    int cur_id_{};
    int first_not_null_id_{};
    bool fective_boolean = false;

    void OperationImpl(const std::string &first_number, std::string &second_number, char operation) {
        first_num_ = Init(first_number);
        second_num_ = Init(second_number);

        if (operation == '>' || operation == '<' || operation == '=') {
            Compare(operation);
        } else if (operation == '+') {
            std::vector <long long> ans = GetSum(first_num_, second_num_);
            PrintNumber(ans);
        } else if (operation == '-') {
            bool is_correct = true;
            std::vector <long long> ans = GetSub(first_num_, second_num_, is_correct);
            if (is_correct) {
                PrintNumber(ans);
            } else {
                std::cout << "Error" << std::endl;
            }
        } else if (operation == '*') {
            std::vector <long long> ans = GetMult(first_num_, second_num_);
            PrintNumber(ans);
        } else if (operation == '/') {
            bool is_correct = true;
            std::vector <long long> ans = GetDiv(first_num_, second_num_, is_correct);
            if (!is_correct) {
                std::cout << "Error" << std::endl;
            } else {
                PrintNumber(ans);
            }
        } else {
            bool is_correct = true;
            std::vector <long long> ans = GetBinPow(first_num_, second_num_, is_correct);
            if (!is_correct) {
                std::cout << "Error" << std::endl;
            } else {
                PrintNumber(ans);
            }
        }
    }

    std::vector <long long> Init(const std::string &str) {
        std::vector <long long> number;
        int str_size = str.size();
        first_not_null_id_ = 0;
        if (str[0] == '0') {
            for (int i = 0; i < str_size; ++i) {
                if (str[i] == '0') {
                    ++first_not_null_id_;
                } else {
                    break;
                }
            }
            if (first_not_null_id_ == str_size) {
                number.push_back(0);
                return number;
            }
        }
        if (str[0] == '0' && str_size == 1) {
            number.push_back(0);
        } else {
            cur_id_ = str_size - 1;
            while (cur_id_ >= first_not_null_id_) {
                long long next = GetNext(str, cur_id_);
                number.push_back(next);
            }
        }
        return number;
    }

    long long GetNext(const std::string &str, int &cur_id) {
        long long num = 0;
        int digits_cnt = 0;
        while (cur_id_ >= first_not_null_id_ && digits_cnt < digits_in_numeral_sys_) {
            num += (str[cur_id] - '0') * std::pow(limb_base_, digits_cnt);
            ++digits_cnt;
            --cur_id_;
        }
        return num;
    }

    static int CompareImpl(const std::vector <long long> &first_num, const std::vector <long long> &second_num) { // 0 - equal, 1 : 1st > 2nd, 2 else
        int first_num_size = first_num.size();
        int second_num_size = second_num.size();
        if (first_num_size != second_num_size) {
            return first_num_size > second_num_size ? 1 : 2;
        } else {
            for (int i = first_num_size - 1; i >= 0; --i) {
                if (first_num[i] != second_num[i]) {
                    return first_num[i] > second_num[i] ? 1 : 2;
                }
            }
        }
        return 0;
    }

    void Compare(char operation) {
        if (operation == '=') {
            int result = CompareImpl(first_num_, second_num_);
            if (result == 0) {
                std::cout << "true" << std::endl;
            } else {
                std::cout << "false" << std::endl;
            }
            return;
        }
        if (operation == '>') {
            int result = CompareImpl(first_num_, second_num_);
            if (result == 1) {
                std::cout << "true" << std::endl;
            } else {
                std::cout << "false" << std::endl;
            }
            return;
        }
        if (operation == '<') {
            int result = CompareImpl(first_num_, second_num_);
            if (result == 2) {
                std::cout << "true" << std::endl;
            } else {
                std::cout << "false" << std::endl;
            }
            return;
        }
    }

    static int GetNum(const std::vector <long long> &num, int pos, int size_of_num) {
        if (pos < size_of_num && pos >= 0) {
            return num[pos];
        }
        return 0;
    }

    std::vector <long long> GetSum(std::vector <long long> &first_num, std::vector <long long> &second_num) {
        std::vector <long long> ans;
        int first_num_size = first_num.size();
        int second_num_size = second_num.size();
        int size = std::max(first_num_size, second_num_size);
        long long transfer = 0;
        for (int i = 0; i < size; ++i) {
            long long sum = GetNum(first_num, i, first_num_size) +
                            GetNum(second_num, i, second_num_size) + transfer;
            ans.push_back(sum % base_);
            transfer = sum / base_;
        }
        if (transfer) {
            ans.push_back(transfer);
        }
        return ans;
    }

    static void ClearNulls(std::vector <long long> &num) {
        int num_size = num.size();
        while (num_size > 1 && num[num_size - 1] == 0) {
            num.pop_back();
            --num_size;
        }
    }

    std::vector <long long> GetSub(std::vector <long long> &first_num, std::vector <long long> &second_num, bool &is_correct) {
        int compare = CompareImpl(first_num, second_num);
        std::vector <long long> ans;
        if (compare == 0) {
            ans.push_back(0);
            return ans;
        } else {
            if (compare == 2) {
                is_correct = false;
            }
            long long transfer = 0;
            int first_num_size = first_num.size();
            int second_num_size = second_num.size();
            for (int i = 0; i < first_num_size; ++i) {
                long long sub = GetNum(first_num, i, first_num_size) -
                                GetNum(second_num, i, second_num_size) - transfer;
                transfer = 0;
                if (sub < 0) {
                    sub += base_;
                    transfer = 1;
                }
                ans.push_back(sub);
            }
            ClearNulls(ans);
            return ans;
        }
    }

    std::vector <long long> GetMult(std::vector <long long> &first_num, std::vector <long long> &second_num) { // not Karatsuba
        int first_num_size = first_num.size();
        int second_num_size = second_num.size();
        std::vector <long long> ans(first_num_size + second_num_size);
        for (int i = 0; i < first_num_size; ++i) {
            long long transfer = 0;
            for (int j = 0; j < second_num_size; ++j) {
                ans[i + j] += first_num[i] * second_num[j] + transfer;
                transfer = ans[i + j] / base_;
                ans[i + j] %= base_;
            }
            if (transfer) {
                ans[i + second_num_size] = transfer;
            }
        }
        ClearNulls(ans);
        return ans;
    }

    std::vector <long long> GetDiv(std::vector <long long> &first_num, std::vector <long long> &second_num, bool &is_correct) {
        std::vector <long long> is_zero;
        is_zero.push_back(0);
        int compare_result = CompareImpl(second_num, is_zero);
        if (compare_result == 0) {
            is_correct = false;
            return is_zero;
        }
        compare_result = CompareImpl(first_num, second_num);
        std::vector <long long> ans;
        if (compare_result == 0) {
            ans.push_back(1);
            return ans;
        } else if (compare_result == 2) {
            ans.push_back(0);
            return ans;
        } else {
            std::vector <long long> next_num = Init("0");
            ans.resize(first_num.size());
            int first_num_size = first_num.size();
            for (int i = first_num_size - 1; i >= 0; --i) {
                next_num.insert(next_num.begin(), first_num[i]);
                if (next_num.back() == 0) {
                    next_num.pop_back();
                }
                long long need = 0;
                long long left = 0;
                long long right = base_;
                while (left <= right) {
                    long long mid = (left + right) / 2;
                    std::vector <long long> mid_big_num = Init(std::to_string(mid));
                    std::vector <long long> cur = GetMult(second_num, mid_big_num);
                    if (CompareImpl(cur, next_num) != 1) {
                        need = mid;
                        left = mid + 1;
                    } else {
                        right = mid - 1;
                    }
                }
                ans[i] = need;
                std::vector <long long> need_big_num = Init(std::to_string(need));
                std::vector <long long> mult = GetMult(second_num, need_big_num);
                std::vector <long long> delta = GetSub(next_num, mult, fective_boolean);
                std::swap(next_num, delta);
            }
            ClearNulls(ans);
            return ans;
        }
    }

    std::vector <long long> DivBigOnShort(std::vector <long long> &num, long long divider) {
        long long transfer = 0;
        int num_size = num.size();
        for (int i = num_size - 1; i >= 0; --i) {
            long long div = num[i] + transfer * base_;
            num[i] = div / divider;
            transfer = div % divider;
        }
        ClearNulls(num);
        return num;
    }

    static bool NumberMod2(const std::vector <long long> &num) {
        return num[0] % 2 == 0;
    }

    std::vector <long long> GetBinPow(std::vector <long long> first_num, std::vector <long long> second_num, bool &is_correct) {
        int first_num_size = first_num.size();
        int second_num_size = second_num.size();
        std::vector <long long> ans = Init("1");
        if (first_num[0] == 0 && second_num[0] == 0 && first_num_size == 1 && second_num_size == 1) {
            is_correct = false;
            return ans;
        } else {
            std::vector <long long> zero = Init("0");
            while (CompareImpl(second_num, zero) != 0) {
                if (!NumberMod2(second_num)) {
                    ans = GetMult(ans, first_num);
                }
                first_num = GetMult(first_num, first_num);
                second_num = DivBigOnShort(second_num, 2);
            }
            return ans;
        }
    }

    void PrintNumber(std::vector <long long> &number) {
        int num_size = number.size();
        int j = num_size - 1;
        for (; j >= 0; --j) {
            if (j == num_size - 1) {
                std::cout << number[j];
            } else {
                int num = number[j];
                if (num == 0) {
                    for (int i = 0; i < digits_in_numeral_sys_; ++i) {
                        std::cout << 0;
                    }
                } else {
                    for (int i = 0; i < digits_in_numeral_sys_; ++i) {
                        if (i < digits_in_numeral_sys_ - (int)(std::log10(num) + 1)) {
                            std::cout << 0;
                        } else {
                            std::cout << num;
                            break;
                        }
                    }
                }
            }
        }
        std::cout << std::endl;
    }

    void ClearImpl() {
        first_num_.clear();
        second_num_.clear();
    }

};


int main() {
    std::ios::sync_with_stdio(false);

    TCalculator calculator;
    std::string first_number;
    std::string second_number;
    char operation;

    while (std::cin >> first_number >> second_number >> operation) {
        calculator.PerformOperation(first_number, second_number, operation);
    }

    return 0;
}