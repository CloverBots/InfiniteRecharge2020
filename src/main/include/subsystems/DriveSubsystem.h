/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <frc/ADXRS450_Gyro.h>
#include <frc/Encoder.h>
#include <frc/SpeedControllerGroup.h>
#include <frc/drive/DifferentialDrive.h>
#include <frc/geometry/Pose2d.h>
#include <frc/kinematics/DifferentialDriveOdometry.h>
#include <frc2/command/SubsystemBase.h>
#include <units/units.h>
#include <frc/smartdashboard/smartdashboard.h>
#include <frc/util/color.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>
#include <vector>
#include "rev/CANSparkMax.h"
#include "Constants.h"
#include "LimePIDOutput.h"
#include "LimePIDSource.h"

class DriveSubsystem : public frc2::SubsystemBase {
  private:
  
  frc::PIDController* limePID;
  LimePIDSource* limeSource;
  LimePIDOutput* limeOutput;
 public:
  void Drive(double speed, double rotate);

  void TankDriveVolts(units::volt_t left, units::volt_t right);

  void SetDriveReversed(bool reversed) { setDriveReversed = reversed; }

  void ResetEncoders();

  double GetAverageEncoderDistance();

  rev::CANEncoder GetLeftEncoder();
  rev::CANEncoder GetRightEncoder();

  void SetMaxOutput(double maxOutput);

  double GetHeading();

  double GetTurnRate();

  frc::Pose2d GetPose();

  frc::DifferentialDriveWheelSpeeds GetWheelSpeeds();

  void ResetOdometry(frc::Pose2d pose);

  DriveSubsystem();

  void Periodic();
  void SetDriveBreakMode(rev::CANSparkMax::IdleMode idlemode)
  {    
    m_leftLeadMotor.SetIdleMode(idlemode);
    m_rightLeadMotor.SetIdleMode(idlemode);
    m_leftFollowMotor.SetIdleMode(idlemode);
    m_rightFollowMotor.SetIdleMode(idlemode);
  }
  double GetTargetCenter()
  {
    std::shared_ptr<NetworkTable> table = nt::NetworkTableInstance::GetDefault().GetTable("limelight");
    double center = table->GetNumber("tx",0.0);
    return center;
  }
  LimePIDSource* GetLimeSource() { return limeSource; }
  LimePIDOutput* GetLimeOutput() { return limeOutput; }
  frc::PIDController* GetLimePID() { return limePID; }
 private:
  bool setDriveReversed = false;

  rev::CANSparkMax m_leftLeadMotor{DriveConst::leftLeadDeviceID, rev::CANSparkMax::MotorType::kBrushless};
  rev::CANSparkMax m_rightLeadMotor{DriveConst::rightLeadDeviceID, rev::CANSparkMax::MotorType::kBrushless};
  rev::CANSparkMax m_leftFollowMotor{DriveConst::leftFollowDeviceID, rev::CANSparkMax::MotorType::kBrushless};
  rev::CANSparkMax m_rightFollowMotor{DriveConst::rightFollowDeviceID, rev::CANSparkMax::MotorType::kBrushless};
  rev::CANEncoder m_leftEncoder = m_leftLeadMotor.GetEncoder();
  rev::CANEncoder m_rightEncoder = m_rightLeadMotor.GetEncoder();
  frc::SpeedControllerGroup m_leftMotors{m_leftLeadMotor, m_leftFollowMotor};

  frc::SpeedControllerGroup m_rightMotors{m_rightLeadMotor, m_rightFollowMotor};
public:
  frc::DifferentialDrive m_drive{m_leftMotors, m_rightMotors};
private:
  frc::ADXRS450_Gyro m_gyro;

  frc::DifferentialDriveOdometry m_odometry;
  
  const bool kGyroReversed = false;
  
  // Components (e.g. motor controllers and sensors) should generally be
  // declared private and exposed only through public methods.
};
