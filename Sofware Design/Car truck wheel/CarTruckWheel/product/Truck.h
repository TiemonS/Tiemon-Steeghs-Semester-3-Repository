#ifndef TRUCK_H_
#define TRUCK_H_

#include "Car.h"

class Truck : public Car
{
  private:
    int* power;

  public:
	Truck(const std::string& model, const std::string& material, int diameter, int nrWheels, int power);
    /* pre : diameter > 0, nrWheels > 2 and vermogen > 0
       post: A Truck object is created that contains 'power' power and nrWheels
             Wheel objects with the given material and diameter
    */

	virtual ~Truck();
	/* pre : -
	   post: the Truck object is destroyed including all allocated memory
	*/

    Truck(const Truck& other);
    /* pre : -
       post: a Truck object is created with all properties of other, a deep copy is performed
    */

    Truck& operator=(const Truck& other);
    /* pre : -
       post: all properties of this object are replaced with other's properties,
             all previously existing properties are properly cleaned up and new
             properties are deep copied
    */

	int GetPower() const;
	/* pre : -
	   post: returns power
	*/
};
#endif
