#pragma once

#include <iostream>
#include <typeinfo>

class ContextA;

class State {
  /**
   * @var ContextA
   */
 protected:
  ContextA *ContextA_;

 public:
  virtual ~State() {
  }

  void set_ContextA(ContextA *ContextA) {
    this->ContextA_ = ContextA;
  }

  virtual void Handle1() = 0;
  virtual void Handle2() = 0;
};


class ContextA {

 private:
  State *state_;

 public:
  ContextA(State *state) : state_(nullptr) {
    this->TransitionTo(state);
  }
  ~ContextA() {
    delete state_;
  }
  /**
   * The ContextA allows changing the State object at runtime.
   */
  void TransitionTo(State *state) {
    std::cout << "ContextA: Transition to " << typeid(*state).name() << ".\n";
    if (this->state_ != nullptr)
      delete this->state_;
    this->state_ = state;
    this->state_->set_ContextA(this);
  }
  /**
   * The ContextA delegates part of its behavior to the current State object.
   */
  void Request1() {
    this->state_->Handle1();
  }
  void Request2() {
    this->state_->Handle2();
  }
};

/**
 * Concrete States implement various behaviors, associated with a state of the
 * ContextA.
 */

class ConcreteStateA : public State {
 public:
  void Handle1() override;

  void Handle2() override {
    std::cout << "ConcreteStateA handles request2.\n";
  }
};

class ConcreteStateB : public State {
 public:
  void Handle1() override {
    std::cout << "ConcreteStateB handles request1.\n";
  }
  void Handle2() override {
    std::cout << "ConcreteStateB handles request2.\n";
    std::cout << "ConcreteStateB wants to change the state of the ContextA.\n";
    this->ContextA_->TransitionTo(new ConcreteStateA);
  }
};

void ConcreteStateA::Handle1() {
  {
    std::cout << "ConcreteStateA handles request1.\n";
    std::cout << "ConcreteStateA wants to change the state of the ContextA.\n";

    this->ContextA_->TransitionTo(new ConcreteStateB);
  }
}