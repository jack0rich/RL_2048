//
// Created by Jack Rich on 2023/3/18.
//

#include "Board.h"
#include <random>
#include <cmath>
#include <iostream>


int board::rand_index() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 3);
    int rand_num = dis(rd);
    return rand_num;
}

int *board::coordinate() {
    int *position = new int[2];
    position[0] = rand_index();
    position[1] = rand_index();

    return position;
}

int board::rand_zero_one() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution d(0.2);

    int rand_num = d(gen);
    return rand_num;
}

board::Board::Board() {
    int *block1 = coordinate();
    int *block2 = coordinate();

    while (block1[0] == block2[0] && block1[1] == block2[1]) {
        block2 = coordinate();
    }
    this->blocks_array[block1[0]][block1[1]] = 1;

    if (rand_zero_one()) {
        this->blocks_array[block2[0]][block2[1]] = 2;
    }
    else {
        this->blocks_array[block2[0]][block2[1]] = 1;
    }

    this->zero_points = this->every_zero_index(true);
}

board::vec_ar2 *board::Board::every_zero_index(bool init) {
    if(init) {
        auto list = new board::vec_ar2;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; ++j) {
                board::vector2 temp = {i, j};
                if (this->blocks_array[i][j] == 0) {
                    list->push_back(temp);
                }
            }
        }
        return list;
    }
}

board::vec_ar2 *board::Board::every_zero_index(){
    this->zero_points->clear();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; ++j) {
            board::vector2 temp = {i, j};
            if (this->blocks_array[i][j] == 0) {
                this->zero_points->push_back(temp);
            }
        }
    }
}

bool board::remove_point(vec_ar2 &map, vector2 coo) {
    int index = 0;
    for (auto item: map) {
        if (item[0] == coo[0] && item[1] == coo[1]) {
            map.erase(map.begin()+index);
            return true;
        } else {index++;}
    }
    return false;
}

board::reshape_array *board::reshape(square4 array, int dim) {
    /* col:0 row:1 */
    auto reshaped = new board::reshape_array;
    if (dim == 0) {
        reshaped->dim = dim;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                int _ = array[j][i];
                reshaped->array[i].push_back(_);
            }
        }
        return reshaped;
    }
    else if (dim == 1) {
        reshaped->dim = dim;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                int _ = array[i][j];
                reshaped->array[i].push_back(_);
            }
        }
        return reshaped;
    }
    else {
        throw "Dim wrong!";
    }
}

void board::merge(reshape_array *slices, bool forward) {
    /* forward: down & right
        backward: up & left*/
    int score = 0;
    if (forward) {
        for (int i = 0; i < 4; ++i) {
            int p = 4;
            for (int j = 3; j > -1; --j) {
                if (slices->array[i][j] != 0) {
                    if (p == 4) {
                        p = j;
                    }
                    else if (slices->array[i][j] == slices->array[i][p]){
                        slices->is_moved = true;
                        slices->array[i][p] += 1;
                        score += (int) std::pow(2, slices->array[i][p]);
                        slices->array[i][j] = 0;
                        p = 4;
                    }
                    else if (slices->array[i][j] != slices->array[i][p]) {
                        p = j;
                    }
                }
            }
            bool exchange;
            do {
                exchange = false;
                for (int j = 2; j > -1; --j) {
                    if (slices->array[i][j] != 0 && slices->array[i][j+1] == 0) {
                        slices->array[i][j+1] = slices->array[i][j];
                        slices->array[i][j] = 0;
                        slices->is_moved = true;
                        exchange = true;
                    }
                }
            } while (exchange);
        }
    }
    else {
        for (int i = 0; i < 4; ++i) {
            int p = -1;
            for (int j = 0; j < 4; ++j) {
                if (slices->array[i][j] != 0) {
                    if (p == -1) {
                        p = j;
                    }
                    else if (slices->array[i][j] == slices->array[i][p]){
                        slices->is_moved = true;
                        slices->array[i][p] += 1;
                        score += (int) std::pow(2, slices->array[i][p]);
                        slices->array[i][j] = 0;
                        p = -1;
                    }
                    else if (slices->array[i][j] != slices->array[i][p]) {
                        p = j;
                    }
                }
            }
            bool exchange;
            do {
                exchange = false;
                for (int j = 1; j < 4; ++j) {
                    if (slices->array[i][j] != 0 && slices->array[i][j-1] == 0) {
                        slices->array[i][j-1] = slices->array[i][j];
                        slices->array[i][j] = 0;
                        slices->is_moved = true;
                        exchange = true;
                    }
                }
            } while (exchange);
        }
    }
    slices->score = score;
}

int board::Board::right() {
    board::reshape_array *reshaped = board::reshape(this->blocks_array, 1);
    merge(reshaped, true);
    int score = reshaped->score;
    this->cover(reshaped);
    this->is_moved = reshaped->is_moved;
    this->add_new_num();
    delete reshaped;
    reshaped = nullptr;
    return score;
}

int board::Board::down() {
    board::reshape_array *reshaped = board::reshape(this->blocks_array, 0);
    merge(reshaped, true);
    int score = reshaped->score;
    this->cover(reshaped);
    this->is_moved = reshaped->is_moved;
    this->add_new_num();
    delete reshaped;
    reshaped = nullptr;
    return score;
}

int board::Board::left() {
    board::reshape_array *reshaped = board::reshape(this->blocks_array, 1);
    merge(reshaped, false);
    int score = reshaped->score;
    this->cover(reshaped);
    this->is_moved = reshaped->is_moved;
    this->add_new_num();
    delete reshaped;
    reshaped = nullptr;
    return score;
}

int board::Board::up() {
    board::reshape_array *reshaped = board::reshape(this->blocks_array, 0);
    merge(reshaped, false);
    int score = reshaped->score;
    this->cover(reshaped);
    this->is_moved = reshaped->is_moved;
    this->add_new_num();
    delete reshaped;
    reshaped = nullptr;
    return score;
}

void board::Board::display() {
    for (int i = 0; i < 4; ++i) {
        std::cout << '|';
        for (int j = 0; j < 4; ++j) {
            int num = this->blocks_array[i][j] == 0 ? 0 : (int)pow(2, this->blocks_array[i][j]) ;
            std::cout << num << '|';
        }
        std::cout << std::endl;
    }
}

void board::Board::cover(reshape_array *slices) {
    int dim = slices->dim;
    if (dim == 0) {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                this->blocks_array[j][i] = slices->array[i][j];
            }
        }
    }
    else if (dim == 1) {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                this->blocks_array[i][j] = slices->array[i][j];
            }
        }
    }
    else {
        throw "Dim wrong!";
    }
}

int board::rand_zero_point(int zeros) {
    if (zeros == 0) {
        return 0;
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, zeros);
    return dis(gen);

}

void board::Board::add_new_num() {
    if (this->is_moved) {
        this->every_zero_index();
        int num_points = (int)this->zero_points->size();
        if(num_points > 0){
            int rand_index = rand_zero_point(num_points-1);
            int x = (*(this->zero_points))[rand_index][0], y = (*(this->zero_points))[rand_index][1];
            if (rand_zero_one()) {
                this->blocks_array[x][y] = 2;
            } else {
                this->blocks_array[x][y] = 1;
            }
        }
        this->is_moved = false;
    }
}

bool board::Board::is_end() {
    this->every_zero_index();
    if (this->zero_points->empty()){
        for (int i = 0; i < 4; ++i) {
            for (int j = 1; j < 4; ++j) {
                if (this->blocks_array[i][j] == this->blocks_array[i][j-1]) {
                    return false;
                }
            }
        }
        for (int i = 0; i < 4; ++i) {
            for (int j = 1; j < 4; ++j) {
                if (this->blocks_array[j][i] == this->blocks_array[j-1][i]) {
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}







