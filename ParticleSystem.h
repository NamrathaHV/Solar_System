/****** Particle System Class & Particle System Constant Definitions*******/
////////////////////////////////////////////////////////////////////////
/*Solar energetic particles(are high energy particle coming from the sun, 
consist of protons, electrons and HZE ions. Particles gives burning effect for Sun */
///////////////////////////////////////////////////////////////////////
/*This Class is used to represent the particle system.
It contains an array of particle structs. Each with their own translation from the
origin, azimuth, and zenith, as well as velocities and lifetimes.
Always used a constant number of particles (when one dies, it is immediately replaced by another)*/

//#include <cmath>
//#include <math.h>
#include <cstdlib>
#include <time.h>

const int NUMBER_OF_PARTICLES = 3000;
const float MAX_ROTATION = 360;
const float MIN_ROTATION = 0;
const float MIN_TRANS = .005;
const float MAX_TRANS = .03;
const float MIN_DELTA_ROTATION = 0.5;
const float MAX_DELTA_ROTATION = 3.0;
const float MIN_DELTA_SURFACE = .005;
const float MAX_DELTA_SURFACE = .01;
const int MIN_LIFETIME = 10;
const int MAX_LIFETIME = 30;
const int DELTA_LIFETIME = 1;


struct Particle {
	//Used For Randomizing The Initial Position of the Particle
	float azimuthRotation, zenithRotation, surfaceTranslationFactor;

	//Used For Randomizing The Delta or Change In Position of Each Particle
	float deltaAz, deltaZe, deltaSurface;

	//Lifetime In "Refreshes"
	int lifetime;
};

class ParticleSystem {

public:
	//Gets the next particle in the array
	Particle getNextParticle();
	//Modify all particles by their deltas, 
	// destroy old ones and create new ones ,if the lifetime is 0
	void updateAll();
	int getNumberOfParticles();
	ParticleSystem();

private:
	//Marker used by getNextParticle to keep track of position.
	int currentParticle;

	//Array of all particle structs
	Particle particles[NUMBER_OF_PARTICLES];

	//This is your random number generator from the last program
	float generateRandomNumber(float lower, float upper);

	//Get a new Particle struct with new randomized values.
	Particle generateNewParticle();

};

//constructor-creates 3000 particles and stored in array of particles
ParticleSystem::ParticleSystem() {
	currentParticle = 0;
	for (int i = 0; i < NUMBER_OF_PARTICLES; i++) {
		particles[i] = generateNewParticle();
	}
}

//creates new particle by initailising azimuthal rotation,zenithal rotation , surface translation factor, delta values and lifetime.
Particle ParticleSystem::generateNewParticle() {
	Particle newParticle;
	newParticle.azimuthRotation = generateRandomNumber(MIN_ROTATION, MAX_ROTATION);
	newParticle.zenithRotation = generateRandomNumber(MIN_ROTATION, MAX_ROTATION);
	newParticle.surfaceTranslationFactor = generateRandomNumber(MIN_TRANS, MAX_TRANS);
	newParticle.deltaAz = generateRandomNumber(MIN_DELTA_ROTATION, MAX_DELTA_ROTATION);
	newParticle.deltaZe = generateRandomNumber(MIN_DELTA_ROTATION, MAX_DELTA_ROTATION);
	newParticle.deltaSurface = generateRandomNumber(MIN_DELTA_SURFACE, MAX_DELTA_SURFACE);
	newParticle.lifetime = (int)generateRandomNumber(MIN_LIFETIME, MAX_LIFETIME);
	return newParticle; //returns the initialized values for all the variables of the structure Particle
}

//generating random number btw lower and upper values
float ParticleSystem::generateRandomNumber(float lower, float upper) { //a,b
	static bool firstTime = true;
	time_t randomNumberSeed;
	//a random seed specifies the start point when a computer generates/program generates a random number sequence.
	if (firstTime) {
		time(&randomNumberSeed); //calculates number of seconds from jan 1st 1970 till current time
		srand(randomNumberSeed); //srand sets the starting point for producing a series of pseudo-random integers
		firstTime = false;          // srand and rand function-header file: <time.h>
	}
	return (lower + ((upper - lower) * (float(rand()) / RAND_MAX))); //(a+((b-a)*(rand()/rand_max))
}

//returns next particle
Particle ParticleSystem::getNextParticle() {
	currentParticle = (currentParticle == NUMBER_OF_PARTICLES) ? 0 : currentParticle;
	return particles[currentParticle++];
}


//returns 3000 (number of max particles)
int ParticleSystem::getNumberOfParticles() {
	return NUMBER_OF_PARTICLES;
}


//funcntion to update the properties od particles
void ParticleSystem::updateAll() {
	//if the particle's lifetime is over(0), replace it with another(create new particle),
	//otherwise update all the appropriate values(decrement life time).
	for (int i = 0; i < NUMBER_OF_PARTICLES; i++) {
		if (particles[i].lifetime == 0) {
			particles[i] = generateNewParticle();
		}
		else {
			particles[i].azimuthRotation += particles[i].deltaAz;
			particles[i].zenithRotation += particles[i].deltaZe;
			particles[i].surfaceTranslationFactor += particles[i].deltaSurface;
			particles[i].lifetime -= DELTA_LIFETIME;
		}
	}
}