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