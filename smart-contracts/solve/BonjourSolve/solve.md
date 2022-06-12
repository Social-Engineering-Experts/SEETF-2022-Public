# [EASY] Bonjour - Solution

**Author**: AtlanticBase

**Category**: Ethereum

Just a simple challenge to get the users accustomed to the environment

You just have to replace the `welcomeMessage` variable to match `Welcome to SEETF`.

Call `setWelcomeMessage("Welcome to SEETF")` to change the welcome message and get the flag.

Code to solve the challenge

```javascript
// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

contract Bonjour {

  string public welcomeMessage;

  constructor() {
    welcomeMessage = "Bonjour";
  }

  function setWelcomeMessage(string memory _welcomeMessage) public {
    welcomeMessage = _welcomeMessage;
  }

  function isSolved() public view returns (bool) {
    return keccak256(abi.encodePacked("Welcome to SEETF")) == keccak256(abi.encodePacked(welcomeMessage));
  }
}

contract BonjourAttack {

    Bonjour public bonjour;

    constructor() {
      bonjour = Bonjour("[Address of Bonjour]");
      bonjour.setWelcomeMessage("Welcome to SEETF");
    }
}
```
