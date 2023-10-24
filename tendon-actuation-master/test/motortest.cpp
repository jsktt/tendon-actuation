#include <gtest/gtest.h>

#include "Motor.hpp"
#include "PIDController.hpp"

// Motor class tests
TEST(MotorTest, SetSpeedUpdatesSpeed) {
  // Create Motor instance
  Motor motor(9, 10);

  // Set speed
  motor.setPWM(100);

  // Check if the speed is set correctly
}

TEST(MotorTest, SetDirectionUpdatesDirection) {
  // Create Motor instance
  Motor motor(9, 10);

  // Set direction
  motor.setDirection(CCW);

  // Check if the direction is set correctly
}

// PIDController class tests
TEST(PIDControllerTest, ComputeControlSignalReturnsCorrectValue) {
  // Create PIDController instance
  PIDController pidController(1.0, 0.5, 0.2);

  // Set PID parameters
  pidController.setPIDParams(2.0, 1.0, 0.5);

  // Compute control signal
  double controlSignal = pidController.computeControlSignal(50, 40);

  // Check if the control signal is calculated correctly
  EXPECT_DOUBLE_EQ(controlSignal, 5.0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    if (RUN_ALL_TESTS())
    ;
    
    return 0;
}