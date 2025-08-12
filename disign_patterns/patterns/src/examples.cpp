#include<iostream>
#include<memory>
#include"../include/examples.h"
#include"../include/factory.hpp"
#include"../include/stratagy.h"
#include"../include/single.h"
#include"../include/adapter.h"
#include"../include/builder.h"
#include"../include/prototype.h"
#include"../include/decorator.h"
#include"../include/proxy.h"
#include"../include/facade.h"
#include"../include/bridge.h"
#include"../include/composite.h"
#include"../include/flyweight.h"


void call_factory() {

   ProductRegistrar<Shoes, NiKeShoes> nikeShoes("nike");
   std::shared_ptr<Shoes> pNiKeShoes(ProductFactory<Shoes>::Instance().GetProduct("nike"));
   pNiKeShoes->Show();


   ProductRegistrar<Clothe, UniqloClothe> adidasShoes("uniqlo");
   std::unique_ptr<Clothe> pUniqloClothe(ProductFactory<Clothe>::Instance().GetProduct("uniqlo"));
   pUniqloClothe->Show();
}

void call_stratagy() {
       
    Context context(std::make_unique<ConcreteStrategyA>());
    std::cout << context.executeStrategy("data") << std::endl;

    context.setStrategy(std::make_unique<ConcreteStrategyB>());
    std::cout << context.executeStrategy("data") << std::endl;
}

void call_single() {
    Singleton& instance_1 = Singleton::get_instance();
    Singleton& instance_2 = Singleton::get_instance();
}
void call_adapter() {

    // using methon-1
    std::shared_ptr<Target>  t1 = std::make_shared<ClassAdapter>();
    t1->request(); //  Adaptee::specificRequest()

    std::unique_ptr<Adaptee> ada(new Adaptee());
    std::unique_ptr<Target> t2(new ObjectAdapter(ada.get()));
    t2->request(); // Adaptee::specificRequest()
}

void call_builder() {
    
    std::unique_ptr<Director> director(new Director);

    //product
	House *house;
 
    // builder A
	std::unique_ptr<AbstractBuilder>  abuilder(new ConcreteBuilderA());
	director->setBuilder(abuilder.get());
	house = director->construct();
	house->printfHouseInfo();
 
    // build B
	std::unique_ptr<AbstractBuilder> bbuilder(new ConcreteBuilderB());
	director->setBuilder(bbuilder.get());
	house = director->construct();
	house->printfHouseInfo();

}

void call_prototype(){
    std::unique_ptr<ConcretePrototypeA> prototypeA = std::make_unique<ConcretePrototypeA>("prototype_A");
    std::unique_ptr<Prototype> cloneA = prototypeA->clone();

    prototypeA->show();
    cloneA->show();
}

void call_decorator() {
    std::unique_ptr<Component>  component(new ConcreteComponent());
    //Component* decoratedComponentA = new ConcreteDecoratorA(component);
    //Component* decoratedComponentB = new ConcreteDecoratorB(decoratedComponentA);
    std::unique_ptr<Component>  decoratedComponentA(new ConcreteDecoratorA(component.get()));
    std::unique_ptr<Component>  decoratedComponentB(new ConcreteDecoratorB(decoratedComponentA.get()));

    decoratedComponentB->operation(); // 调用时会按照装饰器的顺序依次执行功能
    // ...
    
  
}

void call_proxy() {
    RealProxy proxy("user", "pwd");
    proxy.Run();
}

void call_facade() {
     Facade facade;
    facade.Operation();
}

void call_bridge() {
    Implementor* implA = new ConcreteImplementorA();
    Abstraction* absA = new RefinedAbstraction(implA);
    absA->operation(); // 输出: ConcreteImplementorA operation

    Implementor* implB = new ConcreteImplementorB();
    Abstraction* absB = new RefinedAbstraction(implB);
    absB->operation(); // 输出: ConcreteImplementorB operation

    delete implA;
    delete absA;
    delete implB;
    delete absB;
}

void call_composite() {
    // 创建叶子节点
    Leaf* leaf1 = new Leaf("Leaf1");
    Leaf* leaf2 = new Leaf("Leaf2");

    // 创建容器节点
    Composite* composite = new Composite();
    composite->add(leaf1);
    composite->add(leaf2);

    // 创建另一个容器节点
    Composite* composite2 = new Composite();
    composite2->add(new Leaf("Leaf3"));
    composite->add(composite2);

    // 客户端使用
    composite->operation();

    // 释放内存 (实际开发中需要更完善的内存管理)
    delete leaf1;
    delete leaf2;
    delete composite;
    delete composite2;

}
void call_flyweight() {
    //std::map<std::string, Font *> fontPool;
    FlyWeightFactory fly;

    auto f1 = fly.GetFont("1");
    f1->show(); //  key: 1

    auto f2 = fly.GetFont("2");
    f2->show(); //  key: 2

    auto f3 = fly.GetFont("1");
    f3->show(); //  key: 1
}
void call_examples() {
    //call_factory();
    //call_stratagy();
    //call_single();
    //call_adapter();
    //call_builder();
    //call_prototype();
    //call_decorator();
    //call_proxy();
    //call_facade();
    //call_bridge();
    //call_composite();
    call_flyweight();
}






