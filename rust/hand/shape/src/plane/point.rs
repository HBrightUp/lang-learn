use std::{
    ops::{ Add, AddAssign},
    cmp::{ PartialEq },
    fmt::{self, Display},
};
use rand::{ prelude::*};
use serde::{ Serialize, Deserialize, Serializer, Deserializer };
use crate::plane::base::Quadrant;


#[derive(Debug, Default,Serialize, Deserialize)]
pub struct Point {
    pub x: f64,
    pub y: f64,
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

impl AddAssign for Point {
    fn add_assign(&mut self, other: Self) {
        *self = Self {
            x: self.x + other.x,
            y: self.y + other.y,
        };
    }
}

impl PartialEq for Point {
    fn eq(&self, other: &Self) -> bool {
        self.x == other.x && self.y == other.y
    }
}

impl Eq for Point { } 

impl Display for Point {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        writeln!(f, "Point{{ x:{}, y:{} }}", self.x, self.y)
    }
}

impl Point {
    pub fn from(x: f64, y: f64) -> Self {
        Self { x, y }
    }

    pub fn distance(&self, other: &Self) -> f64 {
        ((self.x - other.x).powi(2) + (self.y - other.y).powi(2)).sqrt()
    }

    pub fn distance_x(&self, other: &Self) -> f64 {
        self.x - other.x
    }

    pub fn distance_y(&self, other: &Self) -> f64 {
        self.y - other.y
    }

    pub fn is_equal_x(&self, other: &Self) -> bool {
        self.x  == other.x
    }

    pub fn is_equal_y(&self, other: &Self) -> bool {
        self.y  == other.y
    }

    pub fn quadrant(&self) -> Quadrant {
    
        if self.x == 0.0 && self.y == 0.0 {
            return Quadrant::Origin;
        }

        if self.x == 0.0 {
            return Quadrant::YAxis;
        } 

        if self.y == 0.0 {
            return Quadrant::XAxis;
        }

        if self.x > 0.0 && self.y > 0.0 {
            return Quadrant::First;
        }

        if self.x < 0.0 && self.y > 0.0 {
            return Quadrant::Second;
        }

        if self.x < 0.0 && self.y < 0.0 {
            return Quadrant::Third;
        }

        return Quadrant::Fourth;
    }

    // Tip: Random numbers of  f64 only be in the ranage of [0.0, 1.0) .
    pub fn genrate_rand_point_by_f64() -> Self{

        let mut rng = rand::rng();
        let x: f64 = rng.random::<f64>();
        let y: f64 = rng.random::<f64>();
        Self{ x, y}
    }

    pub fn genrate_rand_point_by_i64() -> Self{
        let mut rng = rand::rng();
        let x = rng.random::<i64>();
        let y = rng.random::<i64>();
        println!("genrate_rand_point_by_i64() x: {x}, y: {y}");

        //Warning: There is a loss of precision here, this is just for testing.
        Self{ x: x as f64, y: y as f64}
    }

    
}


#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_add() {
        let mut p1 = Point{ x: 1.0, y: 2.0};
        let mut p2 = Point{ x: 3.0, y: 4.0};
        assert_eq!(p1 + p2, Point{ x: 4.0, y: 6.0 });
    }

    #[test]
    fn test_distance() {
        let p1 = Point::from(1.0, 1.0);
        let p2 = Point::from(4.0, 5.0);
        assert_eq!(p1.distance(&p2), 5.0);
    }

    #[test]
    fn test_quadrant() {
        let mut p = Point::default();
        assert_eq!(p.quadrant(), Quadrant::Origin);

        p.x = 0.0;
        p.y = 1.0;
        assert_eq!(p.quadrant(), Quadrant::YAxis);

        p.x = 1.0;
        p.y = 0.0;
        assert_eq!(p.quadrant(), Quadrant::XAxis);

        p.x = 1.0;
        p.y = 1.0;
        assert_eq!(p.quadrant(), Quadrant::First);

        p.x = -1.0;
        p.y = 1.0;
        assert_eq!(p.quadrant(), Quadrant::Second);

        p.x = -1.0;
        p.y = -1.0;
        assert_eq!(p.quadrant(), Quadrant::Third);

        p.x = 1.0;
        p.y = -1.0;
        assert_eq!(p.quadrant(), Quadrant::Fourth);
    }

}