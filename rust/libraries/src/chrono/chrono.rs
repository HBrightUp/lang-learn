use chrono::prelude::*;


pub fn enter() {
    get_date_time();
}



fn get_date_time() {

    let utc_time = Utc::now();
    println!("{utc_time}"); //  // 2025-04-29 09:45:38.159885697 UTC

    let local_time = Local::now();
    println!("{local_time}");   // 2025-04-29 17:47:07.721940205 +08:00

    let _dt = Utc.with_ymd_and_hms(2014, 7, 8, 9, 10, 11).unwrap(); // `2014-07-08T09:10:11Z`

}


#[cfg(test)]
mod test {


    #[test]
    fn test_chrono() {

        
    }
}