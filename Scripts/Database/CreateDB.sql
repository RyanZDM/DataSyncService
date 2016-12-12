USE [master]
GO

/****** Object:  Database [OPC]    Script Date: 12/12/2016 4:42:10 PM ******/
DROP DATABASE [OPC]
GO

/****** Object:  Database [OPC]    Script Date: 12/12/2016 4:42:10 PM ******/
CREATE DATABASE [OPC]
 CONTAINMENT = NONE
 ON  PRIMARY 
( NAME = N'OPC', FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL11.CARESTREAM\MSSQL\DATA\OPC.mdf' , SIZE = 5120KB , MAXSIZE = UNLIMITED, FILEGROWTH = 1024KB )
 LOG ON 
( NAME = N'OPC_log', FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL11.CARESTREAM\MSSQL\DATA\OPC_log.ldf' , SIZE = 833024KB , MAXSIZE = 2048GB , FILEGROWTH = 10%)
GO

ALTER DATABASE [OPC] SET COMPATIBILITY_LEVEL = 110
GO

IF (1 = FULLTEXTSERVICEPROPERTY('IsFullTextInstalled'))
begin
EXEC [OPC].[dbo].[sp_fulltext_database] @action = 'enable'
end
GO

ALTER DATABASE [OPC] SET ANSI_NULL_DEFAULT OFF 
GO

ALTER DATABASE [OPC] SET ANSI_NULLS OFF 
GO

ALTER DATABASE [OPC] SET ANSI_PADDING OFF 
GO

ALTER DATABASE [OPC] SET ANSI_WARNINGS OFF 
GO

ALTER DATABASE [OPC] SET ARITHABORT OFF 
GO

ALTER DATABASE [OPC] SET AUTO_CLOSE OFF 
GO

ALTER DATABASE [OPC] SET AUTO_CREATE_STATISTICS ON 
GO

ALTER DATABASE [OPC] SET AUTO_SHRINK OFF 
GO

ALTER DATABASE [OPC] SET AUTO_UPDATE_STATISTICS ON 
GO

ALTER DATABASE [OPC] SET CURSOR_CLOSE_ON_COMMIT OFF 
GO

ALTER DATABASE [OPC] SET CURSOR_DEFAULT  GLOBAL 
GO

ALTER DATABASE [OPC] SET CONCAT_NULL_YIELDS_NULL OFF 
GO

ALTER DATABASE [OPC] SET NUMERIC_ROUNDABORT OFF 
GO

ALTER DATABASE [OPC] SET QUOTED_IDENTIFIER OFF 
GO

ALTER DATABASE [OPC] SET RECURSIVE_TRIGGERS OFF 
GO

ALTER DATABASE [OPC] SET  DISABLE_BROKER 
GO

ALTER DATABASE [OPC] SET AUTO_UPDATE_STATISTICS_ASYNC OFF 
GO

ALTER DATABASE [OPC] SET DATE_CORRELATION_OPTIMIZATION OFF 
GO

ALTER DATABASE [OPC] SET TRUSTWORTHY OFF 
GO

ALTER DATABASE [OPC] SET ALLOW_SNAPSHOT_ISOLATION OFF 
GO

ALTER DATABASE [OPC] SET PARAMETERIZATION SIMPLE 
GO

ALTER DATABASE [OPC] SET READ_COMMITTED_SNAPSHOT OFF 
GO

ALTER DATABASE [OPC] SET HONOR_BROKER_PRIORITY OFF 
GO

ALTER DATABASE [OPC] SET RECOVERY FULL 
GO

ALTER DATABASE [OPC] SET  MULTI_USER 
GO

ALTER DATABASE [OPC] SET PAGE_VERIFY CHECKSUM  
GO

ALTER DATABASE [OPC] SET DB_CHAINING OFF 
GO

ALTER DATABASE [OPC] SET FILESTREAM( NON_TRANSACTED_ACCESS = OFF ) 
GO

ALTER DATABASE [OPC] SET TARGET_RECOVERY_TIME = 0 SECONDS 
GO

ALTER DATABASE [OPC] SET  READ_WRITE 
GO

