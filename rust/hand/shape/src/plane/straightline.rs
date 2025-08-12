use crate::plane::point::Point;
use crate::plane::base::Slope;

pub struct StraightLine {
    p1: Point,
    p2: Point,
}


impl Slope for StraightLine {
    fn slope(&self) -> f64 {
        if self.p1.is_equal_x(&self.p2) {
            return 0.0;
        }

        self.p1.distance_y(&self.p2) / self.p1.distance_x(&self.p2)
    }
}

impl StraightLine {
    pub fn from(x1: f64, y1: f64, x2: f64, y2: f64) -> Self {
        Self{
            p1: Point::from(x1, y1),
            p2: Point::from(x2, y2),
        }
    }

    pub fn from_point(p1: Point, p2: Point) -> Self {
        Self { p1, p2 }
    }
}