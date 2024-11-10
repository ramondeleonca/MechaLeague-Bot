/*
===============================================================================================================
QMC5883LCompass.h Library Azimuth Example Sketch
Learn more at [https://github.com/mprograms/QMC5883Compas]
===============================================================================================================
v0.3 - June 12, 2019
Written by MRPrograms 
Github: [https://github.com/mprograms/]

Release under the GNU General Public License v3
[https://www.gnu.org/licenses/gpl-3.0.en.html]
===============================================================================================================
*/
#include <QMC5883LCompass.h>

QMC5883LCompass compass;

void setup() {
  Serial.begin(9600);
  compass.init();
  // compass.setMode(0x01, 0x0C, 0x00, 0x40);
  // compass.setCalibrationOffsets(624.00, -8296.00, -791.00);
  // compass.setCalibrationScales(2.86, 0.41, 4.34);
  compass.setCalibrationOffsets(-465.00, -8118.00, 1655.00);
  compass.setCalibrationScales(4.42, 0.43, 2.13);
  compass.setMagneticDeclination(4, 40);
  // compass.setSmoothing(5, false);
}

void loop() {
  int a;
  
  // Read compass values
  compass.read();

  // Return Azimuth reading
  a = compass.getAzimuth();
  
  Serial.print("A: ");
  Serial.print(a);
  Serial.println();
  
  delay(10);
}
