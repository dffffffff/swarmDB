# SwarmDB

[![Build Status](https://travis-ci.org/bluzelle/swarmDB.svg?branch=devel)](https://travis-ci.org/bluzelle/swarmDB) [![License](https://img.shields.io/:license-AGPLv3-blue.svg?style=flat-square)](https://github.com/bluzelle/swarmDB/blob/devel/LICENSE) [![Twitter](https://img.shields.io/badge/twitter-@bluzelle-blue.svg?style=flat-square)](https://twitter.com/BluzelleHQ) [![Gitter chat](https://img.shields.io/gitter/room/nwjs/nw.js.svg?style=flat-square)](https://gitter.im/bluzelle)

## ABOUT SWARMDB

Bluzelle brings together the sharing economy and token economy. Bluzelle enables people to rent out their computer storage space to earn a token while dApp developers pay with a token to have their data stored and managed in the most efficient way.

## Getting started with Docker

If you want to deploy your swarm immediately you can use our docker-compose quickstart instructions:

### Install Docker

[Docker Installation Guide](https://docs.docker.com/install/)

1. Setup a local docker-compose swarm with the instructions found [here](https://github.com/bluzelle/docker-swarm-deploy)
2. Run `docker-compose up` in the same directory of your docker-compose.yml. This command will initialize the swarm within your local docker-machine. Full docker-compose documentation can be found [here](https://docs.docker.com/compose/)
3. Nodes are available on localhost port 51010-51012
4. [Connect a test websocket client](https://github.com/bluzelle/swarmDB#testing-locally) with our [websocket API](https://bluzelle.github.io/api/#websocket-api)
5. Create a node server application using our node.js [library](https://github.com/bluzelle/bluzelle-js) and [API](https://bluzelle.github.io/api/)
6. `CTRL-C` to terminate the docker-compose swarm

## Getting started building from source

### Installation - macOSX

**Boost**

```text
$ export BOOST_VERSION="1.67.0"
$ export BOOST_INSTALL_DIR="~/myboost"

$ mkdir -p ~/myboost
$ toolchain/install-boost.sh
```

This will result in a custom Boost install at `~/myboost/1_67_0/`that will not collide with your system's Boost.

**Other dependencies \(Protobuf, CMake\)**

```text
$ brew update && brew install protobuf && brew upgrade cmake
```

**ccache \(Optional\)**

If available, cmake will attempt to use ccache \([https://ccache.samba.org](https://ccache.samba.org)\) to _drastically_ speed up compilation.

```text
$ brew install ccache
```

### Installation - Ubuntu

**Boost**

Open up a console and install the compatible version of Boost:

```text
$ ENV BOOST_VERSION="1.67.0"
$ ENV BOOST_INSTALL_DIR="~/myboost"

$ mkdir -p ~/myboost
$ toolchain/install-boost.sh
```

This will result in a custom Boost install at `~/myboost/1_67_0/`that will not overwrite your system's Boost.

**CMake**

```text
$ mkdir -p ~/mycmake
$ curl -L http://cmake.org/files/v3.11/cmake-3.11.0-Darwin-x86_64.tar.gz | tar -xz -C ~/mycmake --strip-components=1
```

Again, this will result in a custom cmake install into `~/mycmake/` and will not overwrite your system's cmake.

**Protobuf \(Ver. 3 or greater\)**

```text
$ sudo apt-get install pkg-config protobuf-compiler libprotobuf-dev
```

**ccache \(Optional\)**

If available, cmake will attempt to use ccache \([https://ccache.samba.org](https://ccache.samba.org)\) to _drastically_ speed up compilation.

```text
$ sudo apt-get install ccache
```

### Building the Daemon with CLion IDE

Ensure that you set your cmake args to pass in:

```text
-DBOOST_ROOT:PATHNAME=~/myboost/1_67_0/
```

The project root can be directly imported into CLion.

### Building the Daemon from Command Line Interface \(CLI\)

Here are the steps to build the Daemon and unit test application from the command line:

#### OSX

```text
$ mkdir build
$ cd build
$ cmake -DBOOST_ROOT:PATHNAME=~/myboost/1_67_0/ ..
$ sudo make install
```

#### Ubuntu

```text
$ mkdir build
$ cd build
$ ~/mycmake/cmake -DBOOST_ROOT:PATHNAME=~/myboost/1_67_0/ ..
$ sudo make install
```

### Deploying the Daemons

#### The Bluzelle Configuration File

The Bluzelle daemon is configured by setting the properties of a JSON
configuration file provided by the user. This file is usually called
*bluzelle.json* and resides in the current working directory. To specify a
different configuration file the daemon can be executed with the -c command
line argument:

    $ swarm -c peer0.json

The configuration file is a JSON format file, as seen in the following example:

    {
        "bootstrap_file": "./peers.json",
        "ethereum": "0xddbd<...>121a",
        "ethereum_io_api_token": "53IW57FSZSZS3QXJUEBYT8F4YZ9IZFXBPQ",
        "listener_address": "127.0.0.1",
        "listener_port": 49152,
        "log_to_stdout": true,
        "uuid": "d6707510-8ac6-43c1-b9a5-160cf54c99f5",
        "max_storage" : "2GB",
        "logfile_dir" : "logs/",
        "logfile_rotation_size" : "64K"
        "logfile_max_size" : "640K",
        "debug_logging" : false
    }

where the properties are:

- "bootstrap_file" - the path to a file containing the list of peers in the swarm that this node will be participating in. See below.
- "debug_logging" - set this value to true to include debug level log messages in the logs
- "ethereum" - is your Etherium block chain address, used to pay for transactions.
- "ethereum_io_api_token" - this is used to identify the SwarmDB daemon to Etherscan Developer API (see https://etherscan.io/apis). Use the given value for now, this  property may be moved out the config file in the future.
- "listener_address" - the ip address that SwarmDB will use
- "listener_port" - the socket address where SwarmDB will listen for protobuf and web socket requests.
- "log_to_stdout" - directs SwarmDB to log output to stdout when true.
- "logfile_dir" - location of log files (default: logs/)
- "logfile_max_size" - approx. maximum combined size of the logs before deletion occurs (default: 512K)
- "logfile_rotation_size" - approximate size of log file must be before rotation (default: 64K)
- "max_storage" - the approximate maximum limit for the storage that SwarmDB will use in the current instance (default: 2G)
- "uuid" - the universally unique identifier that this instance of SwarmDB will use to uniquely identify itself.

All size entries use the same notation as storage: B, K, M, G & T or none
(bytes)


#### The Bluzelle Bootstrap File

The bootstrap file, identified in the config file by the "bootstrap_file"
parameter, see above, provides a list of other nodes in the the swarm that the
local instance of the SwarmDB daemon can communicate with. Note that this may
not represent the current quorum so if it is sufficently out of date, you may
need to find the current list of nodes.

The booststrap file format a JSON array, containing JSON objects describing
nodes as seen in the following example:

    [
        {
            "host": "127.0.0.1",
            "http_port": 9082,
            "name": "peer0",
            "port": 49152,
            "uuid": "d6707510-8ac6-43c1-b9a5-160cf54c99f5"
        },
        {
            "host": "127.0.0.1",
            "http_port": 9083,
            "name": "peer1",
            "port": 49153,
            "uuid": "5c63dfdc-e251-4b9c-8c36-404972c9b4ec"
        },
        ...
        {
            "host": "127.0.0.1",
            "http_port": 9083,
            "name": "peer1",
            "port": 49153,
            "uuid": "ce4bfdc-63c7-5b9d-1c37-567978e9b893a"
        }
    ]

where the Peer object parameters are:
- "host" - the IP address associated with the external node
- "http_port" - the HTTP port on which the external node listens for HTTP client requests.
- "name" - the human readable name that the external node uses
- "port" - the socket address that the external node will listen for protobuf and web socket requests.
- "uuid" - the universally unique identifier that the external node uses to uniquely identify itself.

Please ensure that a JSON object representing the local node is also included
in the array of peers.


#### Steps to setup Daemon configuration files:

1. Create each of the JSON files below in swarmDB/build/output/, where the swarm executable resides. \(bluzelle.json, bluzelle2.json, bluzelle3.json, peers.json\).
2. Create an account with Etherscan: [https://etherscan.io/register](https://etherscan.io/register)
3. Create an Etherscan API KEY by clicking Developers -&gt; API-KEYs.
4. Add your Etherscan API KEY Token to the configuration files.
5. Modify the `listener_address` to use your local interface IP.

   ```text
   $ ifconfig en1
   en1: flags=8863<UP,BROADCAST,SMART,RUNNING,SIMPLEX,MULTICAST> mtu 1500
    inet6 fe80::1837:c97f:df86:c36f%en1 prefixlen 64 secured scopeid 0xa
   -->>inet 192.168.0.34 netmask 0xffffff00 broadcast 192.168.0.255
    nd6 options=201<PERFORMNUD,DAD>
    media: autoselect
    status: active
   ```

In the above case, the IP address of the local interface is `192.168.0.34`.

If you do not see `inet <ipaddress>`, run `ifconfig` and comb through manually to find your local IP address.

1. Modify the `ethereum` address to be an Ethereum mainnet address that contains tokens or use the sample address provided below.

**Requirements:**

* You must provide a valid Ethereum address with a balance &gt; 0 and an Etherscan API key.
* A unique ID \(uuid\) must be specified for each daemon.
* Listener address and port must match peer list.

Configuration files for Daemon:

```text
// debug_logging is an optional setting (default is false)

// bluzelle.json
{
  "listener_address" : "127.0.0.1",
  "listener_port" : 50000,
  "ethereum" : "0xddbd2b932c763ba5b1b7ae3b362eac3e8d40121a",
  "ethereum_io_api_token" : "**********************************",
  "bootstrap_file" : "./peers.json",
  "uuid" : "60ba0788-9992-4cdb-b1f7-9f68eef52ab9",
  "debug_logging" : true,
  "log_to_stdout" : false
}

// bluzelle2.json
{
  "listener_address" : "127.0.0.1",
  "listener_port" : 50001,
  "ethereum" : "0xddbd2b932c763ba5b1b7ae3b362eac3e8d40121a",
  "ethereum_io_api_token" : "**********************************",
  "bootstrap_file" : "./peers.json",
  "uuid" : "c7044c76-135b-452d-858a-f789d82c7eb7",
  "debug_logging" : true,
  "log_to_stdout" : true
}

// bluzelle3.json
{
  "listener_address" : "127.0.0.1",
  "listener_port" : 50002,
  "ethereum" : "0xddbd2b932c763ba5b1b7ae3b362eac3e8d40121a",
  "ethereum_io_api_token" : "**********************************",
  "bootstrap_file" : "./peers.json",
  "uuid" : "3726ec5f-72b4-4ce6-9e60-f5c47f619a41",
  "debug_logging" : true,
  "log_to_stdout" : true
}

// peers.json
[
  {"name": "peer1", "host": "127.0.0.1", "port": 50000, "http_port: : 8080, "uuid" : "60ba0788-9992-4cdb-b1f7-9f68eef52ab9"},
  {"name": "peer2", "host": "127.0.0.1", "port": 50001, "http_port: : 8081, "uuid" : "c7044c76-135b-452d-858a-f789d82c7eb7"},
  {"name": "peer3", "host": "127.0.0.1", "port": 50002, "http_port: : 8082, "uuid" : "3726ec5f-72b4-4ce6-9e60-f5c47f619a41"}
]
```

1. Deploy your swarm of Daemons. From the swarmDB/build/output/ directory, run:

```text
$ ./swarm -c bluzelle.json
$ ./swarm -c bluzelle2.json
$ ./swarm -c bluzelle3.json
```

## Integration Tests With Bluzelle's Javascript Client

### Installation - macOSX

#### Homebrew

```text
$ /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```

#### Node

```text
$ brew install node
```

#### Yarn

```text
$ brew install yarn
```

### Installation - Ubuntu

#### NPM

```text
$ sudo apt-get install npm
```

#### Update NPM

```text
$ sudo npm install npm@latest -g
```

#### Yarn

```text
$ curl -sS https://dl.yarnpkg.com/debian/pubkey.gpg | sudo apt-key add -
echo "deb https://dl.yarnpkg.com/debian/ stable main" | sudo tee /etc/apt/sources.list.d/yarn.list

$ sudo apt-get update && sudo apt-get install yarn
```

### Running Integration Tests

Script to clone _bluzelle-js_ repository and copy template configuration files or run tests with your configuration files.

```text
$ qa/integration-tests setup // Sets up template configuration files
```

```text
$ qa/integration-tests // Runs tests with configuration files you've created
```

## Testing Locally

```text
$ cd scripts

Follow instructions in readme.md
```

#### Connectivity Test

```text
$ ./crud -n localhost:50000 status
Sending : 
{
    "transaction_id": 4283375944065669395, 
    "bzn-api": "status"
}
------------------------------------------------------------

Response: 
{
	"bzn-api" : "status",
	"module" : 
	[
		{
			"name" : "raft",
			"status" : 
			{
				"state" : "candidate"
			}
		}
	],
	"transaction_id" : 4283375944065669395,
	"version" : "0.0.0-desk"
}

------------------------------------------------------------
```

#### Create

```text
$ ./crud -n localhost:50000 create -u myuuid -k mykey -v myvalue
  Sending: db {
    header {
      db_uuid: "myuuid"
      transaction_id: 1149205427773053859
    }
    create {
      key: "mykey"
      value: "myvalue"
    }
  }

  ------------------------------------------------------------

  redirecting to leader at 127.0.0.1:50002...

  Sending: db {
    header {
      db_uuid: "myuuid"
      transaction_id: 8606810256052859786
    }
    create {
      key: "mykey"
      value: "myvalue"
    }
  }

  ------------------------------------------------------------

  Response: 
  header {
    db_uuid: "myuuid"
    transaction_id: 8606810256052859786
  }

  ------------------------------------------------------------
```

#### Read

```text
$ ./crud -n localhost:50000 read -u myuuid -k mykey
Sending: db {
  header {
    db_uuid: "myuuid"
    transaction_id: 638497919636113033
  }
  read {
    key: "mykey"
  }
}

------------------------------------------------------------

Response: 
header {
  db_uuid: "myuuid"
  transaction_id: 638497919636113033
}
resp {
  value: "myvalue"
}

------------------------------------------------------------
```

#### Update

```text
$ ./crud -n localhost:50000 update -u myuuid -k mykey -v mynewvalue
Sending: db {
  header {
    db_uuid: "myuuid"
    transaction_id: 7847882878681328930
  }
  update {
    key: "mykey"
    value: "mynewvalue"
  }
}

------------------------------------------------------------

redirecting to leader at 127.0.0.1:50002...

Sending: db {
  header {
    db_uuid: "myuuid"
    transaction_id: 2491234936151888566
  }
  update {
    key: "mykey"
    value: "mynewvalue"
  }
}

------------------------------------------------------------

Response: 
header {
  db_uuid: "myuuid"
  transaction_id: 2491234936151888566
}

------------------------------------------------------------
```

#### Delete

```text
$ ./crud -n localhost:50000 delete -u myuuid -k mykey
Sending: db {
  header {
    db_uuid: "myuuid"
    transaction_id: 8470321215009858819
  }
  delete {
    key: "mykey"
  }
}

------------------------------------------------------------

redirecting to leader at 127.0.0.1:50002...

Sending: db {
  header {
    db_uuid: "myuuid"
    transaction_id: 8085312586421869529
  }
  delete {
    key: "mykey"
  }
}

------------------------------------------------------------

Response: 
header {
  db_uuid: "myuuid"
  transaction_id: 8085312586421869529
}

------------------------------------------------------------
```
#### Subscribe
```text
$ ./crud -n localhost:50000 subscribe -u myuuid -k mykey
Sending: 
db {
  header {
    db_uuid: "myuuid"
    transaction_id: 2808384922078102053
  }
  subscribe {
    key: "mykey"
  }
}

------------------------------------------------------------

Response: 
header {
  db_uuid: "myuuid"
  transaction_id: 2808384922078102053
}
resp {
}

------------------------------------------------------------

Waiting....

Response: 
header {
  db_uuid: "myuuid"
  transaction_id: 2808384922078102053
}
resp {
  update {
    key: "mykey"
    value: "mynewvalue"
  }
}

------------------------------------------------------------

Waiting....
```
#### Adding or Removing A Peer

```text
Nodes are added to, or removed from, the network with the add_peer and
remove_peer commands, sent to a leader via WebSocket protocol with the
following JSON objects:

    Adding a peer:

    {
        "bzn-api":"raft",
        "cmd":"add_peer",
        "data":{
            "peer":{
                "host":"<HOST-URL>",
                "http_port":<HTTPPORT>,
                "name":"<NODE-NAME>",
                "port":<PORT>,
                "uuid":"<UUID>"
            }
        }
    }

    The "name" object, <NODE-NAME>, can be a human readable name for the node, "Fluffy" for example.
    The "uuid" object must be a universally unique identifer that uniquely identifies the node within the swarm, or any
    other swarm. This value can be generated online at a site like: https://www.uuidgenerator.net/

    Remove an existing peer:

    {
        "bzn-api":"raft",
        "cmd":"remove_peer",
        "data":{
            "uuid":"<UUID>"
        }
    }

Given a swarm of nodes, a new node can be added via the command line with a
WebSocket client such as wscat (https://www.npmjs.com/package/wscat).

Start the node that you want to add to the swarm, remember that the local peers
list must include the information for the local node for your node to be able
to start. When your node does start, it will not be able to participate in the
swarm.

Create your add_peer JSON object, and use wscat to send it to the swarm leader:

    $ wscat -c  http://<leader-address>:<port>
    connected (press CTRL+C to quit)
    >{"bzn-api":"raft","cmd":"add_peer","data":{"peer":{"host":"104.25.178.61","http_port":84,"name":"peer3","port":49154,"uuid":"7dda1fcb-d494-4fc1-8645-a14056d13afd"}}}
    >
    disconnected
    $

the new node will now start participating in the swarm.

To remove the node, create a remove_peer JSON object, and use wscat to send it
to the swarm leader:

    $ wscat -c  http://<leader-address>:<port>
    connected (press CTRL+C to quit)
    >{"bzn-api" : "raft", "cmd" : "remove_peer", "data" : { "uuid" : "7dda1fcb-d494-4fc1-8645-a14056d13afd" }}
    >
    disconnected
    $

and the node will be removed from the peer list.
```

#### Help & Options

```text
$ ./crud --help
usage: crud [-h] [-p] -n NODE
            {status,create,read,update,delete,has,keys,size,subscribe} ...

crud

positional arguments:
  {status,create,read,update,delete,has,keys,size,subscribe}
    status              Status
    create              Create k/v
    read                Read k/v
    update              Update k/v
    delete              Delete k/v
    has                 Determine whether a key exists within a DB by UUID
    keys                Get all keys for a DB by UUID
    size                Determine the size of the DB by UUID
    subscribe           Subscribe and monitor changes for a key

optional arguments:
  -h, --help            show this help message and exit
  -p, --use_pbft        Direct message to pbft instead of raft

required arguments:
  -n NODE, --node NODE  node's address (ex. 127.0.0.1:51010)
```
