---
layout: default
title: How Does a Custom Memory Allocator Work?
---

- How is memory retrieved from the OS?
- Why not simply use malloc/free?
- How is memory safety and performance guaranteed?
- Why a layer of virtual memory is good?
- What happens when more memory is needed?
- What happens when the memory is not used anymore?

Let's answer these questions by understanding the caveats of existing memory models and 
writing our own memory allocator from scratch in C.

# Table of Contents
{% for part in site.parts %} - [{{part.title}}]({{site.baseurl}}{{part.url}})
{% endfor %}