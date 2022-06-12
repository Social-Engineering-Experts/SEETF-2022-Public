// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

contract DuperSuperSafeSafe {

  address private owner;
  mapping(uint => bytes32) private secret_passphrases;
  uint timestamp;

  constructor(bytes32 _secret_passphrase, bytes32 _secret_passphrase_2) payable {
    owner = msg.sender;
    timestamp = block.timestamp;
    secret_passphrases[0] = _secret_passphrase;
    secret_passphrases[1] = _secret_passphrase_2;
  }

  receive() external payable {}

  modifier restricted() {
    require(
      msg.sender == owner,
      "This function is restricted to the contract's owner"
    );
    _;
  }

  modifier passwordProtected(bytes32 _secret_passphrase, bytes32 _secret_passphrase_2, uint _timestamp) {
    require(keccak256(abi.encodePacked(secret_passphrases[0], secret_passphrases[1], timestamp)) == keccak256(abi.encodePacked(_secret_passphrase, _secret_passphrase_2, _timestamp)), "Wrong secret passphrase");
    _;
  }


  function changeOwner(address _newOwner) public {
    if (tx.origin != msg.sender) {
      owner = _newOwner;
    }
  }

  function changeSecretPassphrase(bytes32 _new_secret_passphrase, bytes32 _new_secret_passphrase_2, bytes32 _secret_passphrase, bytes32 _secret_passphrase_2, uint _timestamp) public restricted passwordProtected(_secret_passphrase, _secret_passphrase_2, _timestamp) {
    secret_passphrases[0] = _new_secret_passphrase;
    secret_passphrases[1] = _new_secret_passphrase_2;
    timestamp = block.timestamp;

  }

  function withdrawFunds(uint _amount, bytes32 _secret_passphrase, bytes32 _secret_passphrase_2, uint _timestamp) external payable restricted passwordProtected(_secret_passphrase, _secret_passphrase_2, _timestamp) {
    require(balanceOf(msg.sender) >= _amount, "Not enough funds");
    payable(address(msg.sender)).transfer(_amount);
  }

  function balanceOf(address _addr) public view returns (uint balance) {
    return address(_addr).balance;
  }

  function isSolved() public view returns (bool) {
    return balanceOf(address(this)) == 0;
  }

}
