//
// Created by Jack Rich on 2023/3/18.
//

#ifndef RL_2048_BOARD_H
#define RL_2048_BOARD_H

#include <array>
#include <vector>
#include <stack>

namespace board {

    typedef std::array<std::array<int, 4>, 4> square4;
    typedef std::array<int, 2> vector2;
    typedef std::vector<vector2> vec_ar2;
    typedef std::array<std::vector<int>, 4> slices;

    typedef struct _re {
        slices array;
        int dim;
        int score = 0;
        bool is_moved = false;
    }reshape_array;

    int rand_index();
    int *coordinate();
    int rand_zero_one();
    bool remove_point(vec_ar2 &map, vector2 coo);
    reshape_array *reshape(square4 array, int dim);
    void merge(reshape_array *slices, bool forward);
    int rand_zero_point(int zeros);

    class Board {
    public:
        square4 blocks_array = {0};
        vec_ar2 *zero_points;
        bool is_moved = false;

        Board();
        vec_ar2 *every_zero_index(bool init);
        vec_ar2 *every_zero_index();
        void display();
        int right();
        int down();
        int left();
        int up();
        void cover(reshape_array *slices);
        void add_new_num();
        bool is_end();
    };
}

#endif //RL_2048_BOARD_H
