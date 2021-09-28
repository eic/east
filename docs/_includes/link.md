{%- assign link=site.data.links | where: "name", include.name | first -%}
[{{ include.tag }}]({{ link.url }}){:target="_blank"}