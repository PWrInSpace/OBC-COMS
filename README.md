# 🛰️ OBC-COMMS | Satellite Communication Subsystem

[![Firmware - STM32H5](https://img.shields.io/badge/Firmware-STM32H5-blue?logo=stmicroelectronics)](https://www.st.com/en/microcontrollers-microprocessors/stm32h5-series.html)
[![Protocol - LoRa](https://img.shields.io/badge/Protocol-LoRa-green)](https://www.semtech.com/products/wireless-rf/lora-core/sx1280)

The **OBC-COMMS** is a high-performance communication board designed for long-range data telemetry. It leverages the **Semtech SX1280** LoRa transceiver and features a complex RF frontend designed for high reliability and maximum spatial coverage.

---

## 📡 System Architecture

The board acts as the primary wireless bridge between the **Main Control Board (MCB)** and the ground station, utilizing an advanced RF path to ensure signal integrity.

### RF Frontend & Signal Path
The heart of the OBC-COMMS is its sophisticated RF chain:
* **Transceiver:** Semtech SX1280 (2.4GHz LoRa/FLRC).
* **Power Splitter:** Distributes the RF signal to **4 separate antennas** for omnidirectional coverage.
* **RF Switch Logic:** * **RX Path:** Dedicated **LNA** (Low Noise Amplifier) for high-sensitivity reception.
    * **TX Path:** High-gain **Power Amplifier (PA)** for long-distance transmission.
* **Diagnostics:** Integrated **MAX2201** for real-time **VSWR** (Voltage Standing Wave Ratio) and power detection to monitor antenna health.

### Data & Logging
* **Inter-Board Interface:** Full-duplex **UART** communication with the MCB (Main Control Board).
* **Storage:** Integrated **SD Card** slot for high-frequency telemetry logging and "black box" data recovery.

---

## 🛠️ Hardware Stack

| Component | Description |
| :--- | :--- |
| **MCU** | STM32H5 Series (Cortex-M33) |
| **RF Module** | Semtech SX1280 (LoRa 2.4GHz) |
| **RF Protection** | MAX2201 Power/VSWR Detector |
| **Frontend** | Custom PA/LNA with 1:4 Power Splitter |
| **Storage** | MicroSD via SPI/SDMMC |
| **Interface** | UART (Interrupt/DMA driven) |

---

## 💻 Software Architecture

The firmware is built on **CMSIS-RTOS v2 (FreeRTOS)** to handle concurrent tasks efficiently.

### Zero-Copy Buffer Management
To optimize memory usage and minimize CPU overhead, the system implements a **Buffer Pool Method** using RTOS Message Queues:
* **FreeQueue:** Holds pointers to available, empty memory buffers.
* **TakenQueue:** Holds pointers to buffers filled with data ready for processing (UART or LoRa).
* **Flow:** A task takes a pointer from the `FreeQueue`, fills it with data (via DMA), and pushes the pointer to the `TakenQueue`. Once processed, the pointer is returned to the `FreeQueue`, avoiding expensive `memcpy` operations.



### Tasks
* **SX1280 Task:** Manages the LoRa state machine, IRQ handling, and CAD (Channel Activity Detection). Based on the [libsx128x](https://github.com/ryankurte/libsx128x) implementation.
* **SD Task:** Monitors the `TakenQueue` to perform thread-safe logging of telemetry data to the SD Card.
* **UART Task:** Handles bidirectional communication with the MCB using DMA-backed circular buffers.

---

## 📚 Libraries & Credits
The RF communication core is based on the [libsx128x](https://github.com/ryankurte/libsx128x) library by Ryan Kurte. This project extends that functionality with:
* STM32H5 HAL integration.
* Custom RF frontend switching logic (PA/LNA).
* RTOS-based buffer pool management.

---

## 🚀 Getting Started

### Prerequisites
* **Toolchain:** `arm-none-eabi-gcc`
* **Build System:** `CMake` (v3.22+)
* **Framework:** STM32CubeH5 HAL / CMSIS-RTOS v2

### Building
```bash
# Clone the repository
git clone [https://github.com/your-org/OBC-COMS.git](https://github.com/your-org/OBC-COMS.git)
cd OBC-COMS

# Configure & Build
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build