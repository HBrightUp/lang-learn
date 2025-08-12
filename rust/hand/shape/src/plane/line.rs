use crate::plane::point::Point;
use crate::plane::base::{ LineRelation, Slope};


#[derive(Debug, Default)]
pub struct Line{
    p1: Point,
    p2: Point,
}

impl PartialEq for Line {
    fn eq(&self, other: &Self) -> bool {
        self.p1 == other.p1 && self.p2 == other.p2
    }
}

impl Eq for Line { }

impl Slope for Line {
    fn slope(&self) -> f64 {
        if self.p1.is_equal_x(&self.p2) {
            return 0.0;
        }

        self.p1.distance_y(&self.p2) / self.p1.distance_x(&self.p2)
    }
}

impl Line {
    pub fn from(x1: f64, y1: f64, x2: f64, y2: f64) -> Self {
        Self {
            p1: Point::from(x1, y1),
            p2: Point::from(x2, y2),
        }
    }

    pub fn from_point(p1: Point, p2: Point) -> Self {
        Self{
            p1,
            p2,
        }
    }

    // length of a line
    pub fn length(&self) -> f64 {
        self.p1.distance(&self.p1)
    }

    pub fn relationship(&self, other: &Self) -> LineRelation {
        if self.eq(other) {
            return LineRelation::Coincide;
        }

        if self.slope() == other.slope() {
            return LineRelation::Parallel;
        }

        if self.p1.x.max(self.p2.x)  < other.p1.x.min(other.p2.x)  ||
            self.p1.y.max(self.p2.y)  < other.p1.y.min(other.p2.y)  ||
            other.p1.x.max(other.p2.x)  < self.p1.x.min(self.p2.x)  ||
            other.p1.y.max(other.p2.y)  < self.p1.y.min(self.p2.y) {
                return LineRelation::None;
        }

        return LineRelation::Cross;
    }

    pub fn get_intersection(&self, other: &Self) -> Option<Point> {
        assert_eq!(self.relationship(&other), LineRelation::Cross);

        if self.relationship(&other) != LineRelation::Cross {
            return None;
        }

        let a1 = self.p2.y - self.p1.y;
        let b1 = self.p1.x - self.p2.x;
        let c1 = self.p1.x * self.p2.y -self.p2.x * self.p1.y;
        let a2 = other.p2.y - other.p1.y;
        let b2 = other.p1.x - other.p2.x;
        let c2 = other.p1.x * other.p2.y - other.p2.x * other.p1.y;

        let x = (b2*c1 - b1*c2) / (a1*b2 -a2*b1);
        let y = (a1*c2 - a2*c1) / (a1*b2 -a2*b1);

        Some(Point{ x, y})
    }

}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_slope() {
        let line = Line::from(1.0, 1.0, 2.0, 2.0);
        assert_eq!(line.slope(), 1.0);
    }

    #[test]
    fn test_relationship() {
        let line1 = Line::from(1.0, 1.0, 2.0, 2.0);
        let line2 = Line::from(1.0, 1.0, 2.0, 2.0);
        let line3 = Line::from(3.0, 3.0, 4.0, 4.0);
        let line4 = Line::from(1.5, 1.0, 1.0, 2.8);
        let line5 = Line::from(-1.5, -1.0, -1.0, -1.8);

        assert_eq!(line1.relationship(&line2), LineRelation::Coincide);
        assert_eq!(line2.relationship(&line3), LineRelation::Parallel);
        assert_eq!(line2.relationship(&line4), LineRelation::Cross);
        assert_eq!(line2.relationship(&line5), LineRelation::None);
    }

    #[test]
    fn test_get_intersection() {
        let line1 = Line::from(-1.0, -1.0, 2.2, 2.2);
        let line2 = Line::from(0.0, 2.0, 2.0, 0.0);

        let point = line1.get_intersection(&line2);
        assert_eq!(point, Some(Point::from(1.0, 1.0)));
    }
}