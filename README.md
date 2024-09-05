
# HTML Table Manipulation Program

**Course**: Object-Oriented Programming  
**Program**: Software Engineering, Faculty of Mathematics and Informatics (FMI)  
**Year**: 2023/2024  

This program is a task from the Object-Oriented Programming course in the Software Engineering program at FMI for the 2023/2024 academic year. The goal is to manipulate HTML tables via a set of commands.

## Program Description

This program allows you to work with HTML tables by supporting the following HTML tags:

- `<table>`: Defines the table
- `<th>`: Defines a header cell in the table (not necessarily at the top)
- `<tr>`: Defines a row in the table
- `<td>`: Defines a cell in the table

You do not need to handle the attributes of these tags.

The program also supports character entity references, where codes like `&#97;` correspond to the character `a`. 

## Supported Functionalities

### 1. Add a Row
```bash
add <rowNumber> <value1> <value2> ...
```

### 2. Remove a Row
```bash
remove <rowNumber>
```

### 3. Edit a Cell
```bash
edit <rowNumber> <colNumber> <newValue>
```

### 4. Print the Table to Console
```bash
print
```

The rows and columns are numbered starting from 1. The program reads a file containing the HTML code of a table and allows manipulation of the table based on the above commands.

Cells can contain any characters. The program should handle text files where the HTML tags might not always be on separate lines.

## Table Display Format
The program should print the table in the following format:
```text
|*header1*|*header2*| ... |*headerN*|
| data11  | data12  | ... | data1N  |
| data21  | data22  | ... | data2N  |
```
If some rows have fewer cells than others, empty cells should be printed to align the table correctly.

### Example
Given the following HTML file:
```html
<table>
  <tr>
    <th>N&#97me</th>
    <th>Age</th>
    <th>Fn</th>
  </tr>
  <tr>
    <td>Petur Ivanov</td>
    <td>34</td>
    <td>12345</td>
  </tr>
  <tr>
    <td>Maria Petrova</td>
    <td>12</td>
  </tr>
  <tr>
    <td>Stefan Stefanov</td>
    <td></td>
    <th>1234</th>
    <td>Unknown field</td>
  </tr>
</table>
```

The table should be printed as:
```text
|*Name            *|*Age*|*Fn   *|               |
| Petur Ivanov     | 34  | 12345 |               |
| Maria Petrova    | 12  |       |               |
| Stefan Stefanov  |     |*1234 *| Unknown field |
```

## Additional Notes
- You may test table visualization using online tools like [W3Schools HTML Tables](https://www.w3schools.com/html/html_tables.asp).
  
### Program Constraints
- A cell may contain a maximum of 50 characters after converting character entity references to ASCII characters.
- A row may contain up to 15 cells.
- A table may contain up to 100 rows.

### Saving Changes
After manipulating the table, the program must save the modified table back to the originally loaded file in HTML format.

