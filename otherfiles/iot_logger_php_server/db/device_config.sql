-- phpMyAdmin SQL Dump
-- version 4.8.5
-- https://www.phpmyadmin.net/
--
-- Host: localhost:3306
-- Generation Time: Jul 07, 2019 at 07:07 PM
-- Server version: 10.3.14-MariaDB
-- PHP Version: 7.3.2

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `id10062120_devices`
--

-- --------------------------------------------------------

--
-- Table structure for table `device_config`
--

CREATE TABLE `device_config` (
  `config_id` int(11) NOT NULL,
  `device_code_to_update_to` text COLLATE utf8_unicode_ci NOT NULL,
  `device_code_type` text COLLATE utf8_unicode_ci NOT NULL,
  `server_host_address_data` text COLLATE utf8_unicode_ci NOT NULL,
  `server_host_port_data` int(11) NOT NULL,
  `device_min_code_version` text COLLATE utf8_unicode_ci NOT NULL,
  `device_max_code_version` text COLLATE utf8_unicode_ci NOT NULL,
  `available_sensor_count` int(11) NOT NULL,
  `sensor_current_enabled` tinyint(1) NOT NULL,
  `sensor_temperature_enabled` tinyint(1) NOT NULL,
  `sensor_vibration_enabled` tinyint(1) NOT NULL,
  `sensor_current_threshold_normal` float NOT NULL,
  `sensor_current_threshold_alert` float NOT NULL,
  `sensor_current_threshold_warning` float NOT NULL,
  `sensor_current_threshold_critical` float NOT NULL,
  `sensor_vibration_threshold_normal` float NOT NULL,
  `sensor_vibration_threshold_alert` float NOT NULL,
  `sensor_vibration_threshold_warning` float NOT NULL,
  `sensor_vibration_threshold_critical` float NOT NULL,
  `sensor_temperature_threshold_normal` float NOT NULL,
  `sensor_temperature_threshold_alert` float NOT NULL,
  `sensor_temperature_threshold_warning` float NOT NULL,
  `sensor_temperature_threshold_critical` float NOT NULL,
  `sensor_current_sample_time_duration` int(11) NOT NULL,
  `sensor_vibration_sample_time_duration` int(11) NOT NULL,
  `sensor_temperature_sample_time_duration` int(11) NOT NULL,
  `sensor_current_total_time_duration` int(11) NOT NULL,
  `sensor_vibration_total_time_duration` int(11) NOT NULL,
  `sensor_temperature_total_time_duration` int(11) NOT NULL,
  `sensor_current_buzzer_light_notify_level` bit(8) NOT NULL,
  `sensor_temperature_buzzer_light_notify_level` bit(8) NOT NULL,
  `sensor_vibration_buzzer_light_notify_level` bit(8) NOT NULL,
  `development_print_level` bit(64) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Dumping data for table `device_config`
--

INSERT INTO `device_config` (`config_id`, `device_code_to_update_to`, `device_code_type`, `server_host_address_data`, `server_host_port_data`, `device_min_code_version`, `device_max_code_version`, `available_sensor_count`, `sensor_current_enabled`, `sensor_temperature_enabled`, `sensor_vibration_enabled`, `sensor_current_threshold_normal`, `sensor_current_threshold_alert`, `sensor_current_threshold_warning`, `sensor_current_threshold_critical`, `sensor_vibration_threshold_normal`, `sensor_vibration_threshold_alert`, `sensor_vibration_threshold_warning`, `sensor_vibration_threshold_critical`, `sensor_temperature_threshold_normal`, `sensor_temperature_threshold_alert`, `sensor_temperature_threshold_warning`, `sensor_temperature_threshold_critical`, `sensor_current_sample_time_duration`, `sensor_vibration_sample_time_duration`, `sensor_temperature_sample_time_duration`, `sensor_current_total_time_duration`, `sensor_vibration_total_time_duration`, `sensor_temperature_total_time_duration`, `sensor_current_buzzer_light_notify_level`, `sensor_temperature_buzzer_light_notify_level`, `sensor_vibration_buzzer_light_notify_level`, `development_print_level`) VALUES
(0, 'v0.0.3-3-g53a0111', 'DEVT', 'device1-eews.000webhostapp.com', 80, '0.0.3-3', '0.0.3-5', 3, 0, 1, 1, 1, 8, 16, 20, 0.1, 1, 3, 5, 25, 35, 40, 55, 400000, 80000, 0, 1000, 3300, 0, b'11111111', b'11111111', b'11111111', b'0000000000000000000000000000000000000000000000000000000000000000');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `device_config`
--
ALTER TABLE `device_config`
  ADD UNIQUE KEY `config_id` (`config_id`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
