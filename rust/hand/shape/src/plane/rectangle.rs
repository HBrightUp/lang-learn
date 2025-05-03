



 use crate::plane::point::Point;


 // Clockwise: p1 -> p2 -> p3 -> p4
#[derive(Debug, Default)]
pub struct Rectangle{
    p1: Point,
    p2: Point,
    p3: Point,
    p4: Point,
    length: f64,
    width: f64,
}

impl Rectangle {

    pub fn from(x1: f64, y1: f64, x2: f64, y2: f64, x3: f64, y3: f64, x4: f64, y4: f64) -> Self {
        assert!(x1 == x2 && y2 == y3 && x3 == x4 && y4 == y1);
        assert!(x1 != x4);

        Self{
            p1: Point::from(x1, y1),
            p2: Point::from(x2, y2),
            p3: Point::from(x3, y3),
            p4: Point::from(x4, y4),
            length: (x1 - x4).abs(),
            width: (y2 - y1).abs(),
        }
    }

    pub fn area(&self) -> f64 {
        self.width * self.length
    }
}

impl PartialEq for Rectangle {
    fn eq(&self, other: &Self) -> bool {
        self.length == other.length && self.width == other.width
    }
}

impl Eq for Rectangle {}

