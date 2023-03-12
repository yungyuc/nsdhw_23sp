#include "line.hpp"

/* Define the copy constructor */
Line::Line(Line const & other){
    // std::cout << "copy aconstructor" << std::endl; 
    m_coord_x.resize(other.m_coord_x.size());
    m_coord_y.resize(other.m_coord_y.size());
    std::copy(other.m_coord_x.begin(), other.m_coord_x.end(), m_coord_x.begin());
    std::copy(other.m_coord_y.begin(), other.m_coord_y.end(), m_coord_y.begin());
}

/* Define the move constructor */
Line::Line(Line && other){
    // std::cout << "move aconstructor" << std::endl; 
    std::swap(other.m_coord_x, m_coord_x);
    std::swap(other.m_coord_y, m_coord_y);
}

/* Define the copy assignment operator */
Line & Line::operator=(Line const & other){
    if(this == &other){ return *this; } // dont move to self

    // std::cout << "copy assignment operator" << std::endl; 
    m_coord_x.resize(other.m_coord_x.size());
    m_coord_y.resize(other.m_coord_y.size());
    std::copy(other.m_coord_x.begin(), other.m_coord_x.end(), m_coord_x.begin());
    std::copy(other.m_coord_y.begin(), other.m_coord_y.end(), m_coord_y.begin());
    return *this;
}

/* Define the move assignment operator */
Line & Line::operator=(Line && other){
    // std::cout << "move assignment operator" << std::endl; 

    if(this == &other){ return *this; } // dont move to self
    std::swap(other.m_coord_x, m_coord_x);
    std::swap(other.m_coord_y, m_coord_y);
    return *this;
}

Line::Line(size_t size){
    m_coord_x.resize(size);
    m_coord_y.resize(size);
}

size_t Line::size() const {
    return m_coord_x.size();
}

float  Line::x(size_t it) const{
    // std::cout << "set x " << it << std::endl; 
    return m_coord_x.at(it);
}

float & Line::x(size_t it){
    // std::cout << "get x " << it << std::endl; 
    return m_coord_x.at(it);
}

float  Line::y(size_t it) const{
    // std::cout << "set y " << it << std::endl; 
    return m_coord_y.at(it);
}

float & Line::y(size_t it){
    // std::cout << "get y " << it << std::endl; 
    return m_coord_y.at(it);
}