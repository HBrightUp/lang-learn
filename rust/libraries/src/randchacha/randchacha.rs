
use rand::distr::{Distribution, Uniform};
use rand_chacha::{rand_core::SeedableRng, ChaCha8Rng};
use rayon::prelude::*;

pub fn enter() {

    monte_carlo();
    rayon_monte_carlo();
}



fn monte_carlo() {
    let range = Uniform::new(-1.0f64, 1.0).unwrap();
    let mut rng = rand::rng();

    let total = 1_000_000;
    let mut in_circle = 0;

    for _ in 0..total {
        let a = range.sample(&mut rng);
        let b = range.sample(&mut rng);
        if a * a + b * b <= 1.0 {
            in_circle += 1;
        }
    }

    // prints something close to 3.14159...
    println!(
        "π is approximately {}",
        4. * (in_circle as f64) / (total as f64)
    );
}

static SEED: u64 = 0;
static BATCH_SIZE: u64 = 10_000;
static BATCHES: u64 = 1000;

fn rayon_monte_carlo() {
    let range = Uniform::new(-1.0f64, 1.0).unwrap();

    let in_circle = (0..BATCHES)
        .into_par_iter()
        .map(|i| {
            let mut rng = ChaCha8Rng::seed_from_u64(SEED);
            rng.set_stream(i);
            let mut count = 0;
            for _ in 0..BATCH_SIZE {
                let a = range.sample(&mut rng);
                let b = range.sample(&mut rng);
    
                if a * a + b * b <= 1.0 {
                    count += 1;
                }

            }
            count
        })
        .sum::<usize>();

        //Warin:  Why?
        assert_eq!(in_circle, 7852263);


    println!(
        "π is approximately {}",
        4. * (in_circle as f64) / ((BATCH_SIZE * BATCHES) as f64)
    );
}