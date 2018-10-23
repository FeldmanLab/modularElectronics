# Contributing
adapted from the https://github.com/labrad Contributing.md

## Documentation

We use the github wikis for documentation.
If you have commit access, just edit the wiki, no pull request needed.
If you do not have commit access, please file an issue and add the `doc/wiki` tag.
We really appreciate contributions to help improve the documentation!

## Code

TODO:

### Branch naming

* Branches dealing with a specific issue should be called `XX-description` where `XX` is the issue's number and description is a description of the issue.
Note that branches may be worked on by more than one user.

* A special case exists for branches named `u/someUserName`.
These branches are for messing around.
You are allowed to rebase and force push to branches prefixed by `u/yourName`.
Other people work on your branches at their peril.
Use of personal branches for working on issues in the issue tracker is discouraged.


### Commits

TODO:


### Commit messages

Your commit message documents your changes for all time. Take pride in it. Commits should follow this format:

```
Brief one sentence description of the change, using the active present tense.

After one blank line, a paragraph describing the change in more detail, i.e.
giving context of the changes and how they influence code use, why it was
done this way, etc. For very small changes, this may not be needed if the
brief description captures the essence of the change.

Meta data about the commit, such as who reviewed it and what issues it fixes.
```
Example:

```
Refactor quizzwopper to handle doodliwigs

Quizzwopper was assuming that all inputs were gizmos, but needed
special handling to deal with doodliwigs, which have extra fuzzbinkle
attributes internally. The quizzwopper interface is unchanged and
existing code does not need to be changed.

Review: @MisterX
Fixes #42 
```


## Coding Style

### C++
Use Google style guide: [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)

TODO: short list outline



## Dos

* **Do** Remove commented code before committing. Old code is in the version control history if you want to get it back. Until then commented out code just confuses the reader.
* **Do** use reversion commits to undo bad commits on master.
* **Do** add commits to master via merge.

## Do Nots

* **Do NOT** force push to master, ever.
* **Do not** push master until the changes are reviewed and approved.
