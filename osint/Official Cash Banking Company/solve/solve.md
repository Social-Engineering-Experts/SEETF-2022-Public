# Official Cash Banking Company - Solution

**Author**: YongJunLim

**Category**: OSINT

1. Using any URL expander, we can see that https://cutt.ly/ocbc-verify-details redirects to https://oc-bc.bitbucket.io/. Although https://bitbucket.org/oc-bc/ does not reveal any public repositories/projects, https://bitbucket.org/oc-bc/workspace/snippets/ lists a public snippet containing [`todo.md`](https://bitbucket.org/oc-bc/workspace/snippets/9XxLEy/2022-todo).

2. Cloning the snippet and using `git log` lets us view the scammer's email address `usxdqyhjhdjmptx5dacg@protonmail.com`. Sharing commenter access and uploading videos were also mentioned in `todo.md`. This suggests that the scammer uses a Google account in order to use Google Docs and YouTube.

3. Using a tool such as [`Epieos`](https://epieos.com) or [`GHunt`](https://github.com/mxrch/GHunt), we can confirm that the scammer has connected his email address to a Google account. A [public Google Calendar](https://calendar.google.com/calendar/embed?src=usxdqyhjhdjmptx5dacg@protonmail.com) owned by the scammer can also be found. Inside the Google Calendar, the scammer has added a [lunch appointment](https://calendar.google.com/calendar/event?eid=MDViYXJibXY0cXRzNTY1NTd0cTBoa29rZjYgdXN4ZHF5aGpoZGptcHR4NWRhY2dAcHJvdG9ubWFpbC5jb20) at a McDonald's outlet in Yishun. Based on the description about a nearby library within 20 minutes' walking distance, we can deduce the scammer is operating from [Yishun Public Library](https://www.nlb.gov.sg/VisitUs/BranchDetails/tabid/140/bid/320/Default.aspx?branch=Yishun+Public+Library).

Flag: `SEE{769098}`
