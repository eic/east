---
title: "The Team"
layout: base
name: team
---

{% assign team=site.data.team | sort: "full" %}

# The Team
---
<table width="80%">
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

