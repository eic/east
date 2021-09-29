---
title: "The Team"
layout: base
name: team
---

{% assign team=site.data.team | sort: "last" %}

# The Team

---

<table width="40%">

  {% for member in team %}
  <tr class="bland">
  <td>{{ member.first }} {{ member.last }}</td>
  <td>{{ member.email }}</td>
  </tr>
  {% endfor %}

</table>
