# QKD Post-processing Simulator

## Overview

The QKD Post-processing Simulator is a tool designed for simulating quantum key distribution (QKD) post-processing protocols. It primarily focuses on error correction and privacy amplification, using protocols such as the Cascade and Cellular Automata algorithms. The simulator is designed to operate in a Dockerized environment or on a standalone Linux host. It uses RabbitMQ for message exchanges between client and server modules, and supports various configurations to test different QKD scenarios.

## Table of Contents

- [Installation](#installation)
- [Running the Simulator](#running-the-simulator)
  - [Dockerized Environment](#dockerized-environment)
  - [Linux Host](#linux-host)
- [Scripts and Usage](#scripts-and-usage)
  - [Run Tests](#run-tests)
  - [Demo](#demo)
- [Developer Guide](#developer-guide)
  - [RabbitMQ Configuration](#rabbitmq-configuration)
  - [Error Correction Module](#error-correction-module)
  - [Privacy Amplification Module](#privacy-amplification-module)

---

## Installation

To set up the simulator, follow the steps below to install dependencies and configure RabbitMQ for message handling.

1. Update package lists:
   `sudo apt-get update`

2. Install necessary packages:

`sudo apt-get install -y build-essential wget libboost-dev libboost-system-dev libboost-thread-dev libevent-dev librabbitmq-dev git python3 python3-pip libssl-dev snapd`

`sudo snap install cmake --classic`


2. Clone and build the AMQP-CPP library:

```bash
git clone https://github.com/CopernicaMarketingSoftware/AMQP-CPP.git

cd AMQP-CPP

mkdir build

cd build

cmake .. -DAMQP-CPP_BUILD_SHARED=ON -DAMQP-CPP_LINUX_TCP=ON

cmake --build . --target install
```

3. Install Python dependencies:

```bash
pip3 install --no-cache-dir -r requirements.txt
```

4. Build the ErrorCorrection and PrivacyAmplification modules:

```bash
cd ErrorCorrection/Build
cmake ..
make
cd ../../PrivacyAmplification/build
cmake ..
make
```

## Running the Simulator

### Dockerized Environment
The simulator runs as three Docker containers: client, server, and RabbitMQ. Execute the following command to build the containers:

`docker-compose build`

To start the simulation, run:

`docker-compose up`

Note: The environment variables in the `.env` file configure the simulation parameters, including:


`PORT`: RabbitMQ port
`MAN_PORT`: RabbitMQ management port
`USR` and `PW`: RabbitMQ credentials
`QBER`, `NBITS`, `COMPRESSION`: QKD-specific parameters

### Linux Host
Alternatively, the simulator can be run directly on a Linux host:

1. Run the install.sh script on both client and server machines:

`./install.sh`

2. Start a RabbitMQ instance on the local network.

3. Execute the simulator using the provided Python API or command-line interface.

## Scripts and Usage
### Run Tests
The `RUN_TESTS` script allows for batch processing of post-processing tests. Use the Python API to specify parameters and variants:

```python
runMultipleTests('qber', 'all', 0.5, [0.01, 0.05], 10000, 100)
```

Command-line usage:

```bash
python3 RUN_TESTS.py [independent variable] [variant] [compression] [qber/keysize] [keysize/qber] [paBlocks] [port] [man_port] [runs]
```
### Demo
The DEMO script provides a single-run test through a web interface at http://localhost:5000. Use it to customize and run simulations interactively.

`python3 DEMO.py`

## Developer Guide
This section covers the main components and modules of the simulator, along with implementation choices.

### RabbitMQ Configuration
RabbitMQ is used for message exchange between the client and server. The AMQP-CPP library handles message transport and offers flexible connection management. Ensure that adequate memory is allocated for RabbitMQ queues, especially for experiments with high message throughput.

### Error Correction Module
The Error Correction (EC) module uses variations of the Cascade protocol to correct errors in the QKD key. Configuration parameters include:

`N`: Number of Cascade and BICONF iterations
`Initial Block Size`: Starting block size for error correction
`Block Size Function`: Function determining subsequent block sizes
`Shuffle Caching`: Enables block reuse to optimize performance

Each EC session generates a JSON file containing:

- EC running time
- Number of channel uses
- Efficiency and error rate
- Final corrected key

**Criticalities**: The EC module relies on UTF-8 formatting for RabbitMQ, which can introduce conversion issues. Additionally, synchronization between messages must be carefully managed to avoid network errors.

## Privacy Amplification Module

The Privacy Amplification (PA) module applies cellular automata-based compression to the QKD key. The primary parameters are:

`K`: Number of blocks
`M`: Number of bits per block
`N`: Cellular Automata (CA) configuration bits

Execution of PA is managed through a RabbitMQ queue and generates a JSON report containing:

- PA running time
- Initial and amplified keys
- Chosen protocol parameters

**Criticalities**: Splitting the key into smaller vectors and synchronizing RabbitMQ messages can lead to errors, especially when bit sizes are not multiples of 64.
