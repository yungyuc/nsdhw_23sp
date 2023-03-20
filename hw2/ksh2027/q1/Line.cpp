#include "Line.hpp"

Line::Line(){
    this->coor.resize(0);
}

Line::Line(size_t size){
    this->coor.resize(size);
    for(unsigned int i=0; i<coor.size(); i++)
        this->coor[i].resize(2);
}

Line::~Line() {};

size_t Line::size() const {
    return this->coor.size(); 
}

float & Line::x(size_t it){ 
    check_range(it); return this->coor[it][0];
}

float & Line::y(size_t it){ 
    check_range(it); return this->coor[it][1]; 
}

void Line::check_range(size_t it) const { 
    if (it >= this->coor.size()){ 
        throw std::out_of_range("Line index out of range"); 
    } 
}