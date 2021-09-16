---
title: "The Team"
layout: base
name: team
---

{% assign team=site.data.team | sort: "full" %}

# The Team
---
<table border="1" width="30%">
<tr><th>Name</th><th>e-mail</th></tr>
  {% for member in team %}
  <tr>
  <td>
  {{ member.full }}
  </td>
  <td>
  {{ member.email }}
  </td>
  </tr>
  {% endfor %}

</table>

