# corba

For learning purposes

## Arch
- Install `omniorb omniorbpy` packages.
- `IDL C++` generation `omniidl -bcxx <filename>.idl`.

## Reference
- [OmniOrb website](http://omniorb.net/docs.html)

## eg3

- Example 3 from the above reference uses **NameService**.
1. Start the service:
	- `omniNames -start -datadir .`
	- It will output to `stdout` an `ORBInitRef`. Look for `IOR:<ior>` with very long
	output.
2. Start the implementation:
	- Copy the previous `<ior>` and substitute below.
	- `eg3_impl -ORBInitRef NameService=IOR:<ior>`
3. Start the client:
	- Copy the previous `<ior>` and substitute below.
	- `eg3_clt -ORBInitRef NameService=IOR:<ior>`

