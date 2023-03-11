#include <iostream>
#include <vector>

class Line
{
public:
    Line() = default;
    Line(Line const & );
    Line(Line       &&);
    Line & operator=(Line const & );
    Line & operator=(Line       &&);
    Line(size_t size){
        this->m_size = size;
        this->m_x_vector.resize(size);
        this->m_y_vector.resize(size);
    };
    ~Line(){
        if(m_size>0){
            this->m_size = 0;
            this->m_x_vector.clear();
            this->m_x_vector.shrink_to_fit();
            this->m_y_vector.clear();
            this->m_y_vector.shrink_to_fit();
        }
    };
    size_t size() const {return this->m_size;}
    float const & x(size_t it) const;
    float & x(size_t it);
    float const & y(size_t it) const;
    float & y(size_t it);
private:
    // Member data.
    std::vector<float> m_x_vector;
    std::vector<float> m_y_vector;
    size_t m_size = 0;
}; /* end class Line */


Line::Line(Line const & other){
    if (other.size() > 0){
        this->m_x_vector.resize(other.size());
        this->m_y_vector.resize(other.size());
        this->m_size = other.size();
        this->m_x_vector = other.m_x_vector;
        this->m_y_vector = other.m_y_vector;
    }else{
        if(this->m_size > 0){
            this->m_x_vector.clear();
            this->m_x_vector.shrink_to_fit();
            this->m_y_vector.clear();
            this->m_y_vector.shrink_to_fit();
            this->m_size = 0;
        }
    }
}

Line::Line(Line && other){
    std::swap(other.m_size, this->m_size);
    other.m_x_vector.swap(this->m_x_vector);
    other.m_y_vector.swap(this->m_y_vector);
}

Line & Line::operator=(Line const & other){
    if(this == &other){
        return *this;
    }
    if (other.size() > 0){
        this->m_size = other.size();
        this->m_x_vector = other.m_x_vector;
        this->m_y_vector = other.m_y_vector;
    }else{
        if(this->m_size > 0){
            this->m_x_vector.clear();
            this->m_x_vector.shrink_to_fit();
            this->m_y_vector.clear();
            this->m_y_vector.shrink_to_fit();
            this->m_size = 0;
        }
    }
    return *this;
}

Line & Line::operator=(Line && other){
    if(this == &other){
        return *this;
    }
    std::swap(other.m_size, this->m_size);
    other.m_x_vector.swap(this->m_x_vector);
    other.m_y_vector.swap(this->m_y_vector);
    return *this;

}
float const & Line::x(size_t size)const{
    if(size > this->m_size){
        throw std::out_of_range("Line index out of range");
    }else{
        return this->m_x_vector[size];
    }
}

float & Line::x(size_t size){
    if(size > this->m_size){
        throw std::out_of_range("Line index out of range");
    }else{
        return this->m_x_vector[size];
    }
}

float const & Line::y(size_t size)const{
    if(size > this->m_size){
        throw std::out_of_range("Line index out of range");
    }else{
        return this->m_y_vector[size];
    }
}

float & Line::y(size_t size){
    if(size > this->m_size){
        throw std::out_of_range("Line index out of range");
    }else{
        return this->m_y_vector[size];
    }
}



int main(int, char **)
{
    Line line(3);
    line.x(0) = 0; line.y(0) = 1;
    line.x(1) = 1; line.y(1) = 3;
    line.x(2) = 2; line.y(2) = 5;

    Line line2(line);
    line2.x(0) = 9;

    std::cout << "line: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line.x(it)
                  << " y = " << line.y(it) << std::endl;
    }

    std::cout << "line2: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line2.x(it)
                  << " y = " << line2.y(it) << std::endl;
    }

    return 0;
}