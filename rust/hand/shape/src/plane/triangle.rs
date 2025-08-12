



use crate::plane::point::Point;


// Clockwise: p1 -> p2 -> p3 
#[derive(Debug, Default)]
pub struct Triangle{
   p1: Point,
   p2: Point,
   p3: Point,
}

impl Triangle {

   pub fn from(x1: f64, y1: f64, x2: f64, y2: f64, x3: f64, y3: f64) -> Self {

       let t = Self{
           p1: Point::from(x1, y1),
           p2: Point::from(x2, y2),
           p3: Point::from(x3, y3),
       };

       assert!(t.p1 != t.p2 && t.p2 != t.p3 && t.p1 != t.p3);

       t
   }

   pub fn area(&self) -> f64 {
        // | (x1(y2 – y3) + x2(y3 – y1) + x3(y1 – y2)) / 2 |
       ((self.p1.x *(self.p2.y - self.p3.y) + self.p2.x*(self.p3.y-self.p1.y) + self.p3.x*(self.p1.y-self.p2.y)) / 2.0).abs()
   }
}

impl PartialEq for Triangle {
   fn eq(&self, other: &Self) -> bool {
       self.area() == other.area()
   }
}

impl Eq for Triangle {}

