# heat

Heat up your laptop (only work with Apple Macbook - tested with M2 Macbook Air only).

## Usage

To build the project, you need bazel,

```
$ bazel build --config opt heat:heat
```

After that, run the command to heat up your laptop

```
$ bazel-bin/heat/heat --method simple --cores 4
```

Check out this [blog post]() for more information.
