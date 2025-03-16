# Sheet 5

- Before we start in the question we need to make sure that we have working linux,
as we will use `pthread` library.

## pre-requests
You should also know how to do the following
- How to compile from linux terminal
<details>
<summary>Hint</summary>
```bash
gcc main.c -o main
```
</details>
- Read arguments from command line
<details>
<summary>Hint</summary>
```bash
./main 2 3 4 5
```
this will give our program 2 3 4 5, also we run the program by calling its name.

---
meanwhile in `C`

```C
int main(int argc, char *argv[]) {
// argc contains the number of arguments
// note that the first element in argv is our program name (path)
// and argc in this case will be 1

// Also argv is split on ' ' spaces
}
```

</details>
- How to get an `integer` from `c-string` (Note that is different from casting)
<details>
<summary>Hint</summary>
`atoi`
</details>

- How to cast `void*` to any thing.
- How to cast `void*` to array of strings.
