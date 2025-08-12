

use std::{
    cmp::{
        Ordering,
    },
};

#[derive(Debug, Hash)]
pub struct Person{
    pub id: u32,
    pub name: String,
    pub age: u8,
}

// overload ==
impl PartialEq<Self> for Person {
    fn eq(&self, other: &Self) -> bool {
        self.id == other.id
    }
}

// overload sort
impl PartialOrd<Self> for Person {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        self.age.partial_cmp(&other.age)
    }
}

 // Overload no require, As long as the id is the same, the person is considered equal.
impl Eq for Person {
    //pass
}

// Overwrite  the rules of 'sort' function.
impl Ord for Person {
    fn cmp(&self, other: &Self) -> Ordering {
        self.id.cmp(&other.id)
    }
}

