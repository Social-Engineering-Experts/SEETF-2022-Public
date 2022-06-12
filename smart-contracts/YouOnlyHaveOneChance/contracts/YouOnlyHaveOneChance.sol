// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

contract YouOnlyHaveOneChance {
    uint256 public balanceAmount;
    address public owner;
    uint256 randNonce = 0;

    constructor() {
        owner = msg.sender;

        balanceAmount =
            uint256(
                keccak256(
                    abi.encodePacked(block.timestamp, msg.sender, randNonce)
                )
            ) %
            1337;
    }

    function isBig(address _account) public view returns (bool) {
        uint256 size;
        assembly {
            size := extcodesize(_account)
        }
        return size > 0;
    }

    function increaseBalance(uint256 _amount) public {
        require(tx.origin != msg.sender);
        require(!isBig(msg.sender), "No Big Objects Allowed.");
        balanceAmount += _amount;
    }

    function isSolved() public view returns (bool) {
        return balanceAmount == 1337;
    }
}
