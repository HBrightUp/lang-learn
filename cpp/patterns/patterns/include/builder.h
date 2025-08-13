#pragma once
#include<iostream>
#include<memory>

// product
class House
{
    public:
        House(){}
        virtual ~House() { std::cout << "House destructor call." << std::endl;}
        void setFloor(std::string foor){
            this->m_floor = foor;
        }
        void setWall(std::string wall){
            this->m_wall = wall;
        }
        void setRoof(std::string roof){
            this->m_roof = roof;
        }
        //打印House信息
        void printfHouseInfo(){
            std::cout << "floor: "<< m_floor << ", wall: " << m_wall << ", roof: "<< m_roof << std::endl;
        }
    private:
        std::string m_floor;
        std::string m_wall;
        std::string m_roof;
};

class AbstractBuilder
{
    public:
        AbstractBuilder(){
            m_pProduce =  std::make_unique<House>();
        }
        virtual ~AbstractBuilder() { std::cout << "AbstractBuilder destructor call." << std::endl;}

        virtual void buildFloor() = 0;
        virtual void buildWall() = 0;
        virtual void buildRoof() = 0;
        virtual House *getHouse() = 0;
    
    protected:
        std::unique_ptr<House> m_pProduce;
};

class ConcreteBuilderA :public AbstractBuilder
{
public:
	ConcreteBuilderA(){
        std::cout << "Constructor ConcreteBuilderA." << std::endl;
	}
    virtual ~ConcreteBuilderA() { std::cout << "ConcreteBuilderA destructor call." << std::endl;}

	void buildFloor(){
		this->m_pProduce->setFloor("Floor_A");
	}
	void buildWall(){
		this->m_pProduce->setWall("Wall_A");
	}
	void buildRoof(){
		this->m_pProduce->setRoof("Roof_A");
	}
	House *getHouse(){
		return this->m_pProduce.get();
	}
};
 
class ConcreteBuilderB :public AbstractBuilder
{
public:
	ConcreteBuilderB(){
        std::cout << "Constructor ConcreteBuilderB." << std::endl;
	}
    virtual ~ConcreteBuilderB() { std::cout << "ConcreteBuilderB destructor call." << std::endl;}

	void buildFloor(){
		this->m_pProduce->setFloor("Floor_B");
	}
	void buildWall(){
		this->m_pProduce->setWall("Wall_B");
	}
	void buildRoof(){
		this->m_pProduce->setRoof("Roof_B");
	}
	House *getHouse(){
		return this->m_pProduce.get();
	}
};

class Director
{
public:
	Director(){}

	void setBuilder(AbstractBuilder *iBuilder){
		this->builder = iBuilder;
	}

	House* construct(){
		builder->buildFloor();
		builder->buildWall();
		builder->buildRoof();
		return builder->getHouse();
	}
private:
	AbstractBuilder *builder;
};

