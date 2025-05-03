use std::{
    ops::{
        Add,Sub,
    },
};

#[derive(PartialEq, Debug)]
pub struct Point{
    pub x: i32,
    pub y: i32,
}
impl Add for Point {
    type Output = Self;

    fn add(self, other: Self) -> Self {
        Self{
            x: self.x + other.x,
            y: self.y + other.y,
        }
    }
}
impl Sub for Point {
    type Output = Self;

    fn sub(self, other: Self) -> Self {
        Self {x: self.x - other.x, y: self.y - other.y}
    }
}