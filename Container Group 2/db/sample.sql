SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";

CREATE TABLE `sample` (
    `text` varchar(100) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

INSERT INTO `sample` (`text`) VALUES
('hello world');


ALTER TABLE `sample`
    ADD PRIMARY KEY (`text`);

COMMIT; 
