---
title: "Contribute to this site"
layout: base
name: contribute
---

* TOC
{:toc}

---

# Overview

This page will be of interest to the collaborators interested in contributing to this
site or involved in its maintenance. In many cases making a contribution can be as easy
as editing a text file.

We use GitHub to manage the content and formatting for this site. Please take a look at the
{% include link.md name='github_east' tag='eAST repository' %}, which
is under the {% include link.md name='github_eic' tag='EIC Software Organization' %}
on GitHub to get an idea of the general organization of the data, layouts and supporing logic.
The content and code for this website is in the ```docs``` folder of the repository.

At the time of writing we are taking advantage of the {% include link.md name='github_east' tag='"GitHub Pages"' %}
functionality of GitHub, which provides hosting and automatic generation of web pages
from the content of the ```docs``` folder using the {% include link.md name='jekyll' tag='Jekyll' %}
website generator.

---

# Jekyll, Markdown and Liquid

The {% include link.md name='jekyll' tag='Jekyll' %} website generator renders
pages written in {% include link.md name='markdown' tag='Markdown' %} format, which
is quite easy to read and edit since it's just plain text for the most part.
In a nutshell, the input for Jekyll is a collection of Markdown-formatted files
and optionally some content formatted in YAML, and the output is a complete
tree of HTML files representing the site to be served on the Web.

Markdown is widely used on GitHub and elsewhere to maintain README files so it's familiar
to most users. Jekyll expects a small section of YAML-formatted data on top of every Markdown
page included in the website. It can be quite minimalistic, e.g.

```yaml
---
title: My eAST Code
name: my_east_code
category: geant4
layout: default
---
```

In fact only two attributes are mandatory - the *layout* which specifies the template use
to render the page, and *name* which is used to reference the page. Any number of templates
can be created for use on the website.

If you decide to contribute to this website we strongly recommend that you install
Jekyll on your machine. The workflow would then consist of
* Cloning or forking the eAST repository
* Making and validating changes locally using Jekyll
* git - commit and push

Since Jekyll is an application written in Ruby language
installing Ruby is a requirement. **Knowledge of Ruby is absolutely not required**
since all development takes place in the very basic Markdown and YAML environment.
Installation instructions can be found on
on the [installation page](https://jekyllrb.com/docs/installation/) of the Jekyll website.
Once the installation is done, you can run a development web server locally:

```bash
bundle exec jekyll serve
```

Assuming you are in the ```docs``` folder of the eAST repository,
once Jekyll has been started you can view the web site by connecting to `localhost:4000`.
Changes made to files locally are immediately compiled and reflected on the
displayed site (at the next page load). The optimal workflow is to make changes and debug
entirely locally before doing a 'git commit' and pushing to GitHub. Once material
is pushed to GitHub the instance of the website hosted on that portal will be automatically
recompiled and serve. This may take a few minutes.

Users who whish to develop or maintain templates for this website need to get
basic familiarity with the {% include link.md name='liquid' tag='Liquid' %} template
language. It is easy to read and learn.

---

# Formatting

We aim to provide a uniform look and feel across the site.

Blockquotes are used to put emphasis on quoted text. The standard
way of doing it in Markdown is to prepend the lines that need this
formatting with the '>' character. For example:

```
>Blockquote!
```

...will produce
>Blockquote!

Language-aware code highlighting capability has been integrated and
can be easily used via the fenced code blocks i.e. blocks delimited by
triple backticks, with an optional shortcut for the programming language name.
For example,

    ```bash
    export foo="foo"
    echo $foo
    ls -l .
    ```

will be rendered as:

```bash
export foo="foo"
echo $foo
ls -l .
```

and

    ```python
    def my_function():
      print("Hello from a function")
    ```

will show as:

```python
def my_function():
  print("Hello from a function")
```

For C++, the opening line should be triple backticks followed by "c++"
similar to examples above and the code will be rendered accordingly such as:

```c++
class MyClass {       // The class
  public:             // Access specifier
    int myNum;        // Attribute (int variable)
    string myString;  // Attribute (string variable)
};
```
---

# Managing Data

Jekyll is a capable platform when it comes to storing and manipulating structured data.
The data component of the site can reside in the "front matter" section of the Markdown-formatted
files or in separate YAML (or JSON, CSV etc) data sources. The front matter approach works well
for small sites. For scalability, it is recommended to rely mostly on dedicated data files (i.e.
files in the "<a href="https://github.com/eic/east/tree/main/docs/_data" target="_blank">_data</a>" folder)
and keep the content of the front matter sections of individual MD files to a minimum.

YAML (and data in other formats) are parsed into data structures such as lists an arrays,
providing an intuitive way for the developer to combine and process data using 
the {% include link.md name='liquid' tag='Liquid' %} template language. This is often
done using *macros* which are technically include files but are semantically similar to functions.
For example, this site contains a file named
<a href="https://github.com/eic/east/tree/main/docs/_data/links.yml" target="_blank">links.yml</a>
which provides a way to quickly generate URLs on pages using mnemonic names. This is done
using this simple macro:
<a href="https://raw.githubusercontent.com/eic/east/main/docs/_includes/link.md" target="_blank">link.md</a>


