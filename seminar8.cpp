#include <iostream>
#include <array>
#include <vector>
#include <map>
#include <queue>

static const size_t LINE_SIZE = 4;
static const size_t FIELD_SIZE = LINE_SIZE * LINE_SIZE;

using FieldArray = std::array<uint8_t, FIELD_SIZE>;

const FieldArray EMPTY_STATE = {
    0, 0, 0, 0,
    0, 0, 0, 0 ,
    0, 0, 0,  0,
     0, 0, 0,  0
};

const FieldArray GOAL_STATE = {
    1, 2, 3,
    4, 5, 6,
    7, 8, 9,10,11,12,13,14,15,0
};

class FieldState {
public:
    FieldState()  {
        init(EMPTY_STATE);
    }
    FieldState(const FieldState& st) = default;

    FieldState(const FieldArray& arr) {
        init(arr);
    }

    bool operator==(const FieldState& r) const {
        return state_array == r.state_array;
    }

    bool operator<(const FieldState& r) const {
        return state_array < r.state_array;
    }

    ~FieldState() {
    }

    std::vector<FieldState> GetChildren() const {
        std::vector<FieldState> result;

        if (zero_pos >= LINE_SIZE) { // up
            FieldState state(*this);
            state.moveUp();
            result.push_back(state);
        }
        if (zero_pos < (FIELD_SIZE - LINE_SIZE)) { // down
            FieldState state(*this);
            state.moveDown();
            result.push_back(state);

        }
        if ((zero_pos + 1) % LINE_SIZE != 0) { // right
            FieldState state(*this);
            state.moveRight();
            result.push_back(state);
        } 
        if (zero_pos % LINE_SIZE != 0) { // left
            FieldState state(*this);
            state.moveLeft();
            result.push_back(state);
        }

        return result;
    }

    void print() const {
        for (int i = 0; i < state_array.size(); ++i) {
            std::cout << (int)state_array[i] << " ";
            if ((i + 1) % LINE_SIZE == 0) {
                std::cout << std::endl;
            }
        }
        std::cout << "zero_pos: " << (int)zero_pos << std::endl;
    }
private:
    /*
    FieldState getMoveUp() const {
        FieldState state(*this);
        std::swap(state.state_array[state.zero_pos], state.state_array[state.zero_pos - LINE_SIZE]);
        state.zero_pos = state.zero_pos - LINE_SIZE;
    }
    */

   void moveUp() {
        std::swap(state_array[zero_pos], state_array[zero_pos - LINE_SIZE]);
        zero_pos = zero_pos - LINE_SIZE;
   }

   void moveDown() {
        std::swap(state_array[zero_pos], state_array[zero_pos + LINE_SIZE]);
        zero_pos = zero_pos + LINE_SIZE;
   }

   void moveLeft() {
        std::swap(state_array[zero_pos], state_array[zero_pos - 1]);
        zero_pos = zero_pos - 1;
   }

   void moveRight() {
        std::swap(state_array[zero_pos], state_array[zero_pos + 1]);
        zero_pos = zero_pos + 1;
   }

    void init(const FieldArray& arr) {
        for (int i = 0; i < arr.size(); ++i) {
            state_array[i] = arr[i];
            if (arr[i] == 0) {
                zero_pos = i;
            }
        }
    }

    FieldArray state_array;
    uint8_t zero_pos;   
};

void bfs(const FieldState& start) {
    // vertex -> parent
    std::map<FieldState, FieldState> visited;
    std::queue<FieldState> q;

    const FieldState empty_state(EMPTY_STATE);
    const FieldState goal_state(GOAL_STATE);

    visited.insert(std::make_pair(start, empty_state));
    q.push(start);

    FieldState cur = empty_state;
    while (!q.empty()) {
        
        cur = q.front();
        q.pop();

        //std::cout << "iter" << std::endl;
        //cur.print();

        if (cur == goal_state) {
            break;
        }

        std::vector<FieldState> children = cur.GetChildren();
        //std::cout << "children size: " << children.size() << std::endl;
        for (int i = 0; i < children.size(); ++i) {
            const FieldState& child = children[i];
            if (visited.find(child) == visited.end()) {
                visited.insert(std::make_pair(child, cur));
                q.push(child);
            }
        }
    }
    
    if (cur == goal_state) {
        int steps_count = 0;
        while (!(cur == empty_state)) {
            cur.print();
            cur = visited.find(cur)->second;
            steps_count++;
        }
        std::cout << "Solution found, steps_count=" << steps_count << std::endl;

    } else {
        std::cout << "Path not found, visited size=" << visited.size() << std::endl;
    }
}

int main2() {
    /*
    FieldArray arr = {
        1,2,3,4,
        8,5,6,8,
        10,0,7,12,
        13,14,11,15
    };*/
    FieldArray arr = {
        1,2,3,4,
        5,6,7,8,
        9,10,0,12,
        13,14,11,15
    };
    FieldState start(arr);
    bfs(start);
}
