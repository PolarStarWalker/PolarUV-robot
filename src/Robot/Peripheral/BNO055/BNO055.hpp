#ifndef ROBOT_BNO055_HPP
#define ROBOT_BNO055_HPP
namespace BNO055{
    enum BNO055_REGISTERS : uint8_t {
        PAGE_ID_REG = 0X07,
        CHIP_ID_REG = 0x00,
        ACCEL_REV_ID_REG = 0x01,
        MAG_REV_ID_REG = 0x02,
        GYRO_REV_ID_REG = 0x03,
        SW_REV_ID_LSB_REG = 0x04,
        SW_REV_ID_MSB_REG = 0x05,
        BL_REV_ID_REG = 0X06,

        ACCEL_DATA_X_LSB_REG = 0X08,
        ACCEL_DATA_X_MSB_REG = 0X09,
        ACCEL_DATA_Y_LSB_REG = 0X0A,
        ACCEL_DATA_Y_MSB_REG = 0X0B,
        ACCEL_DATA_Z_LSB_REG = 0X0C,
        ACCEL_DATA_Z_MSB_REG = 0X0D,

        MAG_DATA_X_LSB_REG = 0X0E,
        MAG_DATA_X_MSB_REG = 0X0F,
        MAG_DATA_Y_LSB_REG = 0X10,
        MAG_DATA_Y_MSB_REG = 0X11,
        MAG_DATA_Z_LSB_REG = 0X12,
        MAG_DATA_Z_MSB_REG = 0X13,

        GYRO_DATA_X_LSB_REG = 0X14,
        GYRO_DATA_X_MSB_REG = 0X15,
        GYRO_DATA_Y_LSB_REG = 0X16,
        GYRO_DATA_Y_MSB_REG = 0X17,
        GYRO_DATA_Z_LSB_REG = 0X18,
        GYRO_DATA_Z_MSB_REG = 0X19,

        EULER_H_LSB_REG = 0X1A,
        EULER_H_MSB_REG = 0X1B,
        EULER_R_LSB_REG = 0X1C,
        EULER_R_MSB_REG = 0X1D,
        EULER_P_LSB_REG = 0X1E,
        EULER_P_MSB_REG = 0X1F,

        QUATERNION_DATA_W_LSB_REG = 0X20,
        QUATERNION_DATA_W_MSB_REG = 0X21,
        QUATERNION_DATA_X_LSB_REG = 0X22,
        QUATERNION_DATA_X_MSB_REG = 0X23,
        QUATERNION_DATA_Y_LSB_REG = 0X24,
        QUATERNION_DATA_Y_MSB_REG = 0X25,
        QUATERNION_DATA_Z_LSB_REG = 0X26,
        QUATERNION_DATA_Z_MSB_REG = 0X27,

        LINEAR_ACCEL_DATA_X_LSB_REG = 0X28,
        LINEAR_ACCEL_DATA_X_MSB_REG = 0X29,
        LINEAR_ACCEL_DATA_Y_LSB_REG = 0X2A,
        LINEAR_ACCEL_DATA_Y_MSB_REG = 0X2B,
        LINEAR_ACCEL_DATA_Z_LSB_REG = 0X2C,
        LINEAR_ACCEL_DATA_Z_MSB_REG = 0X2D,

        GRAVITY_DATA_X_LSB_REG = 0X2E,
        GRAVITY_DATA_X_MSB_REG = 0X2F,
        GRAVITY_DATA_Y_LSB_REG = 0X30,
        GRAVITY_DATA_Y_MSB_REG = 0X31,
        GRAVITY_DATA_Z_LSB_REG = 0X32,
        GRAVITY_DATA_Z_MSB_REG = 0X33,

        TEMP_REG = 0X34,

        CALIB_STAT_REG = 0X35,
        SELFTEST_RESULT_REG = 0X36,
        INTR_STAT_REG = 0X37,

        SYS_CLK_STAT_REG = 0X38,
        SYS_STAT_REG = 0X39,
        SYS_ERR_REG = 0X3A,

        UNIT_SEL_REG = 0X3B,

        OPR_MODE_REG = 0X3D,
        PWR_MODE_REG = 0X3E,

        SYS_TRIGGER_REG = 0X3F,
        TEMP_SOURCE_REG = 0X40,

        AXIS_MAP_CONFIG_REG = 0X41,
        AXIS_MAP_SIGN_REG = 0X42,

        SIC_MATRIX_0_LSB_REG = 0X43,
        SIC_MATRIX_0_MSB_REG = 0X44,
        SIC_MATRIX_1_LSB_REG = 0X45,
        SIC_MATRIX_1_MSB_REG = 0X46,
        SIC_MATRIX_2_LSB_REG = 0X47,
        SIC_MATRIX_2_MSB_REG = 0X48,
        SIC_MATRIX_3_LSB_REG = 0X49,
        SIC_MATRIX_3_MSB_REG = 0X4A,
        SIC_MATRIX_4_LSB_REG = 0X4B,
        SIC_MATRIX_4_MSB_REG = 0X4C,
        SIC_MATRIX_5_LSB_REG = 0X4D,
        SIC_MATRIX_5_MSB_REG = 0X4E,
        SIC_MATRIX_6_LSB_REG = 0X4F,
        SIC_MATRIX_6_MSB_REG = 0X50,
        SIC_MATRIX_7_LSB_REG = 0X51,
        SIC_MATRIX_7_MSB_REG = 0X52,
        SIC_MATRIX_8_LSB_REG = 0X53,
        SIC_MATRIX_8_MSB_REG = 0X54,

        ACCEL_OFFSET_X_LSB_REG = 0X55,
        ACCEL_OFFSET_X_MSB_REG = 0X56,
        ACCEL_OFFSET_Y_LSB_REG = 0X57,
        ACCEL_OFFSET_Y_MSB_REG = 0X58,
        ACCEL_OFFSET_Z_LSB_REG = 0X59,
        ACCEL_OFFSET_Z_MSB_REG = 0X5A,

        MAG_OFFSET_X_LSB = 0X5B,
        MAG_OFFSET_X_MSB_REG = 0X5C,
        MAG_OFFSET_Y_LSB_REG = 0X5D,
        MAG_OFFSET_Y_MSB_REG = 0X5E,
        MAG_OFFSET_Z_LSB_REG = 0X5F,
        MAG_OFFSET_Z_MSB_REG = 0X60,

        GYRO_OFFSET_X_LSB_REG = 0X61,
        GYRO_OFFSET_X_MSB_REG = 0X62,
        GYRO_OFFSET_Y_LSB_REG = 0X63,
        GYRO_OFFSET_Y_MSB_REG = 0X64,
        GYRO_OFFSET_Z_LSB_REG = 0X65,
        GYRO_OFFSET_Z_MSB_REG = 0X66,

        ACCEL_RADIUS_LSB_REG = 0X67,
        ACCEL_RADIUS_MSB_REG = 0X68,
        MAG_RADIUS_LSB_REG = 0X69,
        MAG_RADIUS_MSB_REG = 0X6A
    };

    enum PowerMode : uint8_t {
        POWER_MODE_NORMAL = 0X00,
        POWER_MODE_LOWPOWER = 0X01,
        POWER_MODE_SUSPEND = 0X02
    };

    enum OperationMode : uint8_t {
        OPERATION_MODE_CONFIG = 0X00,
        OPERATION_MODE_ACCONLY = 0X01,
        OPERATION_MODE_MAGONLY = 0X02,
        OPERATION_MODE_GYRONLY = 0X03,
        OPERATION_MODE_ACCMAG = 0X04,
        OPERATION_MODE_ACCGYRO = 0X05,
        OPERATION_MODE_MAGGYRO = 0X06,
        OPERATION_MODE_AMG = 0X07,
        OPERATION_MODE_IMUPLUS = 0X08,
        OPERATION_MODE_COMPASS = 0X09,
        OPERATION_MODE_M4G = 0X0A,
        OPERATION_MODE_NDOF_FMC_OFF = 0X0B,
        OPERATION_MODE_NDOF = 0X0C
    };

    struct Data{
        ///{W, I, J, K}
        std::array<double, 4> Quaternions{};
        ///{X, Y, Z}
        std::array<double, 3> EulerAngle{};
        ///{X, Y, Z}
        std::array<double, 3> LinearAcceleration{};
        ///{X, Y, Z}
        std::array<double, 3> MagneticFiled{};
        ///{X, Y, Z}
        std::array<uint8_t, 4> Calibration{};
        int8_t Temperature = 0;
    };
}
#endif
