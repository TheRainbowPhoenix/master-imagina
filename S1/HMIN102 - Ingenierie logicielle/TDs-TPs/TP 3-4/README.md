# Compiler et executer un test avec le terminal

**Compilation:**
```bash
javac -cp .:junit-4.10.jar:harmcrest-core-1.3.jar *.java
```
**Execution:**
```bash
java -cp .:junit-4.10.jar:harmcrest-core-1.3.jar org.junit.runner.JUnitCore OrderedDictionaryTest
```
