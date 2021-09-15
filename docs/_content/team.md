---
title: "The Team"
layout: base
name: team
---

<table width="80%">
  {% for who in site.data.team %}
  <tr>
  <td>
  {{ who.full }}
  </td>
  <td>
  {{ who.email }}
  </td>
  </tr>
  {% endfor %}

</table>

