use crate::plane::point::Point;


#[derive(Debug, Default)]
pub struct Circle{
    origin: Point,
    radius: f64,
}

const CIRCLE_PAI: f64 = 3.14159265;


impl PartialEq for Circle {
    fn eq(&self, other: &Self) -> bool {
        self.radius == other.radius &&
        self.origin == other.origin
    }
}

impl Eq for Circle {}

impl PartialOrd for Circle {
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        self.radius.partial_cmp(&other.radius)
    }
}

//impl Ord for Circle {}

impl Circle {

    fn from( origin: Point, radius: f64 ) -> Self {
        assert!(radius != 0.0);

        Self{
            origin,
            radius,
        }
    }

    fn area(&self) -> f64 {
        CIRCLE_PAI * self.radius.powi(2)
    } 
}

#[cfg(test)] 
mod test {

    use super::*;

    #[test]
    fn test_eq() {
        let c1 = Circle{
            origin: Point::from(1.0, 1.0),
            radius: 2.0,
        };

        let c2 = Circle{
            origin: Point::from(1.0, 1.0),
            radius: 2.0,
        };

        assert_eq!(c1, c2);

        let c3 = Circle {
            origin: Point::from(2.0, 9.0),
            radius: 4.4,
        };

        assert!(c2 != c3);
        assert!(c2 < c3);
    }

}