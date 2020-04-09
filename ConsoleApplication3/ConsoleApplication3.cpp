// ConsoleApplication3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

class interval
{
public:
    int begin;
    int end;
    interval(): begin(1), end(-1) {}
    interval(int a, int b) : begin(a), end(b) {};
    interval(interval const& a) : begin(a.begin), end(a.end) {}
    interval& operator=(const interval a)
    {
        begin = a.begin;
        end = a.end;
    }
    bool operator<(const interval a) const
    {
        if (this->begin == a.begin)
            return this->end < a.end;
        else
            return this->begin < a.begin;
    }
    bool is_empty() const
    {
        return begin > end;
    }

    std::vector<int> to_vector() const
    {
        std::vector<int> answer;
        for (int a = begin; a <= end; ++a)
            answer.push_back(a);
        return answer;
    }
    static interval intersect(interval a, interval b)
    {
        if (a.end < b.begin || b.end < a.begin)
            return interval();
        else if (a.begin <= b.begin && b.end <= a.end)
            return b;
        else if (b.begin <= a.begin && a.end <= b.end)
            return a;
        else if (b.end <= a.begin)
            return interval(b.end, a.begin);
        else //if (a.end <= b.begin)
            return interval(a.end, b.begin);
    }
    /*static pair<interval, interval> subtract(interval a, interval b)
    {
    
    }*/
    static bool isRend(interval pa, int value)
    {
        return value == pa.end + 1;
    }
    static bool isLend(interval pa, int value)
    {
        return value == pa.begin - 1;
    }
    static bool in(interval pa, int value)
    {
        return pa.begin <= value && value <= pa.end;
    }
};

class pack {
private:
    set<interval> interval_list;
    pack(set<interval> Se) : interval_list(Se) {}

    static void merge_ranges(std::set<interval> &container, std::set<interval>::iterator a, std::set<interval>::iterator b)
    {
        interval tmp(a->begin, b->end);
        container.insert(tmp);
        container.erase(a);
        container.erase(b);
    }
    static pack range_extraction(std::vector<int> args) {
        set<interval> interval_list;
        for (auto a = args.begin(); a < args.end(); ++a)
        {
            bool added = false;
            for (auto y = interval_list.begin(); y != interval_list.end(); ++y)
                if (interval::in(*y, *a))
                {
                    added = true;
                    break;
                }
                else if (interval::isRend(*y, *a))
                {
                    auto tmp = interval((*y).begin, *a);
                    interval_list.erase(y);
                    interval_list.insert(tmp);
                    added = true;
                    break;
                }
                else if (interval::isLend(*y, *a))
                {
                    auto tmp = interval(*a, (*y).end);
                    interval_list.erase(y);
                    interval_list.insert(tmp);
                    added = true;
                    break;
                }
            if (!added)
                interval_list.insert(interval(*a, *a));
        }
        renew_intervals(interval_list);
        pack result(interval_list);
        return result;
    }
    static void renew_intervals(std::set<interval>& container)
    {
        for (auto itr = container.begin(); itr != container.end();)
        {
            if (itr->is_empty)
                container.erase(itr++);
            else
                ++itr;
        }
        bool changed = true;
        while (changed)
        {
            changed = false;
            for (auto iter1 = container.begin(), iter2 = ++(container.begin()); iter2 != container.end(); ++iter1, ++iter2)
            {
                if (iter1->end + 1 >= iter2->begin)
                {
                    merge_ranges(container, iter1, iter2);
                    changed = true;
                    break;
                }
            }
        }
    }


public:
    pack():interval_list() {}
    pack(const pack& a):interval_list(set<interval>(a.interval_list)) {}
    pack(std::vector<int> vec) :interval_list(range_extraction(vec).interval_list){}   
    void operator=(pack& a)
    { interval_list = a.interval_list;}
    void operator=(pack&& a)
    {interval_list = std::move(a.interval_list);}
    std::string to_string()
    {
        if (interval_list.size == 0)
            return string("");
        std::string answer = "";
        for (auto it = this->interval_list.begin(); it != this->interval_list.end(); ++it)
            if (it->begin == it->end)
                answer += ',' + std::to_string(it->begin);
            else
                if (it->begin + 1 == it->end)
                    answer += ',' + std::to_string(it->begin) + ',' + std::to_string(it->end);
                else
                    answer += ',' + std::to_string(it->begin) + '-' + std::to_string(it->end);
        answer.erase(answer.begin());
        return answer;
    }
    std::vector<int> to_vector()
    {
        if (interval_list.size == 0)
            return vector<int>();
        std::vector<int> answer;
        for (auto it = this->interval_list.begin(); it != this->interval_list.end(); ++it)
            if (it->begin == it->end)
                answer.push_back(it->begin);
            else
                for (int i = it->begin; i <= it->end; ++i)
                    answer.push_back(i);
        return answer;
    }
    bool contains(int a)
    {
        if (interval_list.size == 0)
            return false;
        for (auto itr = interval_list.begin(); itr != interval_list.end(); ++itr)
            if (interval::in(*itr, a))
                return true;
        return false;
    }
    static pack unit(pack a, pack b)
    {
        std::set<interval> tmp(a.interval_list);
        for (auto itr = b.interval_list.begin(); itr != b.interval_list.end(); ++itr)
            tmp.insert(*itr);
        renew_intervals(tmp);
        return pack(tmp);
    }
    static pack intersect(pack a, pack b)
    {
        if (a.interval_list.size == 0 || b.interval_list.size == 0)
            return pack();
        std::set<interval> answer;
        for (auto f = a.interval_list.begin(); f != a.interval_list.end(); ++f)
            for (auto s = b.interval_list.begin(); s != b.interval_list.end(); ++s)
            {
                interval tmp = interval::intersect(*f, *s);
                if(!tmp.is_empty())
                    answer.insert(tmp);
            }
        renew_intervals(answer);
        return pack(answer);
    }
    /*static pack subtract(pack a, pack b)
    {
        std::set<interval> answer;
        for (auto f = a.interval_list.begin(); f != a.interval_list.end(); ++f)
            for (auto s = b.interval_list.begin(); s != b.interval_list.end(); ++s)
            {
                auto tmp = interval::subtract(*f, *s);
                if (!tmp.first.is_empty())
                    answer.insert(tmp.first);
                if (!tmp.second.is_empty())
                    answer.insert(tmp.second);
            }
        renew_intervals(answer);
        return pack(answer);
    }*/
    /*static pack multi_subtraction(pack a, pack b)
    {
        std::vector<int> unition1 = a.to_vector();
        std::vector<int> unition2 = b.to_vector();
        std::vector<int> tmp;
        for (auto itr = unition1.begin(); itr != unition1.end(); ++itr)
            if (!b.contains(*itr))
                tmp.push_back(*itr);
        for (auto itr = unition2.begin(); itr != unition2.end(); ++itr)
            if (!a.contains(*itr))
                tmp.push_back(*itr);
        return pack(tmp);
    }*/
    int size()
    {
        return this->to_vector().size();
    }
};
int main()
{
    pack a(std::vector<int>{-1,1,3,5,7,9,12});
    pack b(std::vector<int>{-2,0,2,4,6,8,10});
    std::cout << pack::intersect(a,b).to_string() << std::endl;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
