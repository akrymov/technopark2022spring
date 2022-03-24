#include <iostream>
#include <sstream>
#include <assert.h>


template <class T>
class Queue
{
    struct Node
    {
        T value;
        Node* next;
        
        Node(const T& val) : value(val), next(nullptr) {}
    };
public:
    Queue() : head(nullptr), tail(nullptr) {
    }
    
    ~Queue() {
        while (!is_empty()) {
            pop();
        }
    }
    
    Queue(const Queue&) = delete;
    Queue& operator =(const Queue&) = delete;
    
    Queue(Queue&&) = delete;
    Queue& operator =(Queue&&) = delete;
    
    void pop() {
        Node* node = head;
        head = head->next;
        delete node;
        
        if (head == nullptr) {
            tail = nullptr;
        }
    }
    
    void push(const T& elem) {
        Node* node = new Node(elem);
        if (is_empty()) {
            head = tail = node;
            return;
        }
        tail->next = node;
        tail = node;
    }
    
    T& top() {
        return head->value;
    }
    
    bool is_empty() const {
        return head == tail && head == nullptr;
    }
    
private:
    Node* head;
    Node* tail;
};

void testQueue() {
    {
        Queue<int> queue;
        assert(queue.is_empty());
    }
    {
        Queue<int> queue;
        for (int i = 0; i < 100; i++) {
            queue.push(i);
        }
        for (int i = 0; i < 100; i++) {
            assert(queue.top() == i);
            queue.pop();
        }
        assert(queue.is_empty());
    }
}


int run(std::istream& input, std::ostream& output) {
    Queue<int> queue;
    int num = 0;
    input >> num;
    for (int i = 0; i < num; i++) {
        int command = 0;
        int command_value = -1;
        int queue_value = -1;
        input >> command >> command_value;
        switch (command) {
            case 2:
                if (!queue.is_empty()) {
                    queue_value = queue.top();
                    queue.pop();
                }
                if (command_value != queue_value) {
                    output << "NO" << std::endl;
                    return 0;
                }
                break;
            case 3:
                queue.push(command_value);
                break;
            default:
                output << "NO" << std::endl;
                return 0;
        }
    }
    output << "YES" << std::endl;
    return 0;
}

void testLogic() {
    {
        std::stringstream sstr_input;
        sstr_input << "3" << std::endl;
        sstr_input << "3 44" << std::endl;
        sstr_input << "3 50" << std::endl;
        sstr_input << "2 44" << std::endl;
        std::stringstream sstr_output;

        run(sstr_input, sstr_output);
        assert(sstr_output.str() == "YES\n");
    }
    {
        std::stringstream sstr_input;
        sstr_input << "1" << std::endl;
        sstr_input << "2 -1" << std::endl;
        std::stringstream sstr_output;
        
        run(sstr_input, sstr_output);
        assert(sstr_output.str() == "YES\n");
    }
    {
        std::stringstream sstr_input;
        sstr_input << "1" << std::endl;
        sstr_input << "2 100" << std::endl;
        std::stringstream sstr_output;
        
        run(sstr_input, sstr_output);
        assert(sstr_output.str() == "NO\n");
    }
}

int main(int argc, const char * argv[]) {
    //testQueue();
    //testLogic();
    //std::cout << "OK" << std::endl;
    return run(std::cin, std::cout);
}
