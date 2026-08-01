// ===== ECO MODE =====
(function() {
    const body = document.body;
    const toggleBtns = document.querySelectorAll('#eco-toggle, #eco-toggle-mobile');

    function enableEcoMode() {
        body.classList.add('dark-mode');
        
        document.querySelectorAll('*').forEach(el => {
            el.style.animationDuration = '0s';
            el.style.animationDelay = '0s';
            el.style.transitionDuration = '0s';
        });
        
        localStorage.setItem('eco-mode', 'true');
        updateEcoButton(true);
    }

    function disableEcoMode() {
        body.classList.remove('dark-mode');
        
        document.querySelectorAll('*').forEach(el => {
            el.style.animationDuration = '';
            el.style.animationDelay = '';
            el.style.transitionDuration = '';
        });
        
        localStorage.setItem('eco-mode', 'false');
        updateEcoButton(false);
    }

    function toggleEcoMode() {
        body.classList.contains('dark-mode') 
            ? disableEcoMode() 
            : enableEcoMode();
    }

    function updateEcoButton(isActive) {
        toggleBtns.forEach(btn => {
            if (!btn) return;
            
            const icon = btn.querySelector('i');
            const text = btn.querySelector('span');
            
            if (isActive) {
                if(icon) icon.className = 'fas fa-sun';
                if(text) text.textContent = 'Mode Normal';
            } else {
                if(icon) icon.className = 'fas fa-leaf';
                if(text) text.textContent = 'Mode Éco';
            }
        });
    }

    if (localStorage.getItem('eco-mode') === 'true') {
        enableEcoMode();
    }
    
    window.addEventListener('load', () => {
        body.classList.remove('preload');
    });

    toggleBtns.forEach(btn => {
        if (btn) {
            btn.addEventListener('click', toggleEcoMode);
        }
    });
})();


// ===== MOBILE MENU =====
document.addEventListener('DOMContentLoaded', function() {
    const burger = document.querySelector('.burger');
    const sideMenu = document.querySelector('.side-menu');
    const overlay = document.querySelector('.side-menu-overlay');
    
    if (burger && sideMenu && overlay) {
        burger.addEventListener('click', function(e) {
            e.stopPropagation();
            burger.classList.toggle('active');
            sideMenu.classList.toggle('active');
            overlay.classList.toggle('active');
            
            document.body.style.overflow = sideMenu.classList.contains('active') 
                ? 'hidden' 
                : '';
        });
        
        overlay.addEventListener('click', function() {
            burger.classList.remove('active');
            sideMenu.classList.remove('active');
            overlay.classList.remove('active');
            document.body.style.overflow = '';
        });
        
        const menuLinks = sideMenu.querySelectorAll('a');
        menuLinks.forEach(function(link) {
            link.addEventListener('click', function() {
                burger.classList.remove('active');
                sideMenu.classList.remove('active');
                overlay.classList.remove('active');
                document.body.style.overflow = '';
            });
        });
    }
});


// ===== SEARCH FUNCTION =====
document.addEventListener('DOMContentLoaded', function() {
    const searchInput = document.querySelector('.search-input');
    const searchBtn = document.querySelector('.search-btn');
    const searchResults = document.querySelector('.search-results');
    
    const searchData = [
        { 
            title: 'Accueil', 
            url: 'pageprincipale.html', 
            description: 'Page principale de Spark Media - Agence de communication digitale B2B', 
            keywords: ['accueil', 'home', 'principal', 'spark media', 'agence', 'communication', 'digital', 'marketing', 'b2b'] 
        },
        { 
            title: 'Notre Équipe', 
            url: 'a_propos.html', 
            description: 'Découvrez l\'équipe fondatrice : Djabrail, Mohamed et Sofiane', 
            keywords: ['équipe', 'team', 'à propos', 'about', 'membres', 'experts', 'qui sommes-nous', 'fondateurs', 'djabrail', 'mohamed', 'sofiane'] 
        },
        { 
            title: 'Nos Services - Gestion Réseaux Sociaux', 
            url: 'service.html', 
            description: 'Création de contenu, planification, publicité et reporting pour Instagram, LinkedIn, TikTok', 
            keywords: ['services', 'réseaux sociaux', 'création', 'contenu', 'web', 'publicité', 'marketing', 'instagram', 'facebook', 'tiktok', 'linkedin', 'ads', 'community management'] 
        },
        { 
            title: 'Création de Contenu', 
            url: 'service.html', 
            description: 'Visuels professionnels, vidéos courtes, infographies et textes optimisés', 
            keywords: ['création', 'contenu', 'vidéo', 'infographie', 'rédaction', 'seo', 'graphisme', 'design', 'reels', 'stories'] 
        },
        { 
            title: 'Planification & Publication', 
            url: 'service.html', 
            description: 'Stratégie éditoriale et publication régulière avec Hootsuite et Buffer', 
            keywords: ['planification', 'publication', 'stratégie', 'éditoriale', 'hootsuite', 'buffer', 'calendrier'] 
        },
        { 
            title: 'Gestion Publicitaire (Ads)', 
            url: 'service.html', 
            description: 'Campagnes Meta Ads, LinkedIn Ads et Google Ads optimisées', 
            keywords: ['publicité', 'ads', 'meta ads', 'linkedin ads', 'google ads', 'campagne', 'roi', 'a/b testing'] 
        },
        { 
            title: 'Reporting & Analyse', 
            url: 'service.html', 
            description: 'Rapports détaillés sur les performances et l\'engagement', 
            keywords: ['reporting', 'analyse', 'statistiques', 'performances', 'engagement', 'kpi', 'metrics'] 
        },
        { 
            title: 'Nos Offres et Tarifs', 
            url: 'offres.html', 
            description: 'Forfaits Starter, Business et Premium de 1800€ à 3500€/mois', 
            keywords: ['offres', 'prix', 'tarifs', 'forfaits', 'packages', 'abonnement', 'starter', 'business', 'premium', '1800', '2500', '3500', 'euros'] 
        },
        { 
            title: 'Forfait Starter', 
            url: 'offres.html', 
            description: '1800€/mois - 12 posts, 2 réseaux sociaux, idéal pour débuter', 
            keywords: ['starter', 'débutant', '1800', 'premier forfait', 'petit budget'] 
        },
        { 
            title: 'Forfait Business', 
            url: 'offres.html', 
            description: '2500€/mois - 20 posts, 3 réseaux, gestion publicitaire incluse', 
            keywords: ['business', 'moyen', '2500', 'populaire', 'ads inclus'] 
        },
        { 
            title: 'Forfait Premium', 
            url: 'offres.html', 
            description: '3500€/mois - 30 posts, 5 réseaux, service complet avec optimisation IA', 
            keywords: ['premium', 'complet', '3500', 'haut de gamme', 'ia', 'intelligence artificielle'] 
        },
        { 
            title: 'Recrutement - Offres d\'emploi', 
            url: 'recrutement.html', 
            description: 'Rejoignez notre équipe : Community Manager, Créateur Vidéo, Graphiste, Traffic Manager', 
            keywords: ['recrutement', 'emploi', 'carrière', 'jobs', 'stage', 'alternance', 'cdi', 'postuler', 'community manager', 'graphiste', 'développeur', 'traffic manager'] 
        },
        { 
            title: 'Community Manager - CDI', 
            url: 'recrutement.html', 
            description: 'Gestion des réseaux sociaux, création de contenu, 28K-35K€/an', 
            keywords: ['community manager', 'réseaux sociaux', 'cdi', 'emploi', 'canva', 'adobe'] 
        },
        { 
            title: 'Créateur de Contenu Vidéo - CDI', 
            url: 'recrutement.html', 
            description: 'Réalisation vidéos TikTok, Reels, YouTube, 26K-33K€/an', 
            keywords: ['vidéo', 'montage', 'premiere pro', 'after effects', 'tiktok', 'reels'] 
        },
        { 
            title: 'Graphiste Designer - CDD', 
            url: 'recrutement.html', 
            description: 'Création visuels, design UX/UI, identités visuelles, 25K-32K€/an', 
            keywords: ['graphiste', 'designer', 'ux', 'ui', 'photoshop', 'illustrator', 'cdd'] 
        },
        { 
            title: 'Traffic Manager - CDI', 
            url: 'recrutement.html', 
            description: 'Expert en gestion campagnes publicitaires Meta, Google, LinkedIn, 32K-42K€/an', 
            keywords: ['traffic manager', 'ads', 'publicité', 'analytics', 'certifications'] 
        },
        { 
            title: 'Stage Marketing Digital', 
            url: 'recrutement.html', 
            description: 'Assistant marketing digital - 6 mois, gratification légale', 
            keywords: ['stage', 'assistant', 'marketing digital', '6 mois', 'étudiant'] 
        },
        { 
            title: 'Alternance Développeur Web', 
            url: 'recrutement.html', 
            description: 'Développeur Full-Stack en alternance - 12 ou 24 mois', 
            keywords: ['alternance', 'développeur', 'web', 'full-stack', 'html', 'css', 'javascript', 'php'] 
        },
        { 
            title: 'Contact', 
            url: 'contact.html', 
            description: 'Contactez-nous pour un audit gratuit ou un devis personnalisé', 
            keywords: ['contact', 'formulaire', 'email', 'message', 'nous contacter', 'devis', 'rendez-vous', 'audit gratuit'] 
        },
        { 
            title: 'Mentions Légales', 
            url: 'mentionlegale.html', 
            description: 'Mentions légales et informations juridiques', 
            keywords: ['mentions', 'légales', 'juridique', 'cgv', 'conditions', 'rgpd', 'confidentialité'] 
        },
        { 
            title: 'Audit Digital Gratuit', 
            url: 'offres.html', 
            description: 'Obtenez un audit complet de votre présence en ligne (valeur 800€)', 
            keywords: ['audit', 'gratuit', 'analyse', 'diagnostic', 'offre découverte'] 
        }
    ];
    
    function performSearch(query) {
        query = query.toLowerCase().trim();
        
        if (query.length === 0) {
            searchResults.classList.remove('active');
            return;
        }
        
        const results = searchData.map(function(item) {
            let score = 0;
            
            if (item.title.toLowerCase() === query) {
                score += 100;
            } else if (item.title.toLowerCase().includes(query)) {
                score += 50;
            }
            
            if (item.description.toLowerCase().includes(query)) {
                score += 30;
            }
            
            item.keywords.forEach(function(keyword) {
                if (keyword.toLowerCase().includes(query)) {
                    score += 20;
                }
                if (keyword.toLowerCase() === query) {
                    score += 30;
                }
            });
            
            return { item: item, score: score };
        })
        .filter(result => result.score > 0)
        .sort((a, b) => b.score - a.score)
        .slice(0, 8)
        .map(result => result.item);
        
        displayResults(results, query);
    }
    
    function displayResults(results, query) {
        searchResults.innerHTML = '';
        
        if (results.length === 0) {
            searchResults.innerHTML = `
                <div class="search-no-results">
                    <i class="fas fa-search"></i>
                    <p>Aucun résultat pour "${escapeHtml(query)}"</p>
                    <small>Essayez : services, tarifs, contact, emploi, équipe...</small>
                </div>
            `;
            searchResults.classList.add('active');
            return;
        }
        
        results.forEach(function(result) {
            const resultItem = document.createElement('div');
            resultItem.className = 'search-result-item';
            resultItem.innerHTML = `
                <div class="search-result-title">
                    <i class="fas fa-file-alt"></i>
                    ${escapeHtml(result.title)}
                </div>
                <div class="search-result-description">
                    ${escapeHtml(result.description)}
                </div>
            `;
            
            resultItem.addEventListener('click', function() {
                window.location.href = result.url;
                searchResults.classList.remove('active');
            });
            
            searchResults.appendChild(resultItem);
        });
        
        searchResults.classList.add('active');
    }
    
    function escapeHtml(text) {
        const map = { 
            '&': '&amp;', 
            '<': '&lt;', 
            '>': '&gt;', 
            '"': '&quot;', 
            "'": '&#039;' 
        };
        return text.replace(/[&<>"']/g, function(m) { 
            return map[m]; 
        });
    }
    
    let searchTimeout;
    
    if (searchInput) {
        searchInput.addEventListener('input', function() {
            clearTimeout(searchTimeout);
            searchTimeout = setTimeout(function() {
                performSearch(searchInput.value);
            }, 200);
        });
        
        if (searchBtn) {
            searchBtn.addEventListener('click', function(e) {
                e.preventDefault();
                e.stopPropagation();
                
                const query = searchInput.value.toLowerCase().trim();
                if (query.length === 0) return;
                
                const results = searchData
                    .map(function(item) {
                        let score = 0;
                        
                        if (item.title.toLowerCase() === query) {
                            score += 100;
                        } else if (item.title.toLowerCase().includes(query)) {
                            score += 50;
                        }
                        
                        if (item.description.toLowerCase().includes(query)) {
                            score += 30;
                        }
                        
                        item.keywords.forEach(function(keyword) {
                            if (keyword.toLowerCase().includes(query)) {
                                score += 20;
                            }
                            if (keyword.toLowerCase() === query) {
                                score += 30;
                            }
                        });
                        
                        return { item: item, score: score };
                    })
                    .filter(result => result.score > 0)
                    .sort((a, b) => b.score - a.score);
                
                if (results.length > 0) {
                    window.location.href = results[0].item.url;
                } else {
                    alert('Aucun résultat trouvé pour "' + query + '".');
                }
            });
        }
        
        searchInput.addEventListener('keypress', function(e) {
            if (e.key === 'Enter') {
                e.preventDefault();
                clearTimeout(searchTimeout);
                
                const query = this.value.toLowerCase().trim();
                if (query.length === 0) return;
                
                const results = searchData
                    .map(function(item) {
                        let score = 0;
                        
                        if (item.title.toLowerCase() === query) {
                            score += 100;
                        } else if (item.title.toLowerCase().includes(query)) {
                            score += 50;
                        }
                        
                        if (item.description.toLowerCase().includes(query)) {
                            score += 30;
                        }
                        
                        item.keywords.forEach(function(keyword) {
                            if (keyword.toLowerCase().includes(query)) {
                                score += 20;
                            }
                            if (keyword.toLowerCase() === query) {
                                score += 30;
                            }
                        });
                        
                        return { item: item, score: score };
                    })
                    .filter(result => result.score > 0)
                    .sort((a, b) => b.score - a.score);
                
                if (results.length > 0) {
                    window.location.href = results[0].item.url;
                }
            }
        });
        
        searchInput.addEventListener('keydown', function(e) {
            if (e.key === 'Escape') {
                searchResults.classList.remove('active');
                searchInput.blur();
            }
        });
        
        document.addEventListener('click', function(e) {
            const searchContainer = document.querySelector('.search-container');
            if (searchContainer && !searchContainer.contains(e.target)) {
                searchResults.classList.remove('active');
            }
        });
    }
});


// ===== SCROLL TOP BUTTON =====
document.addEventListener('DOMContentLoaded', function() {
    const scrollBtn = document.querySelector('.scroll-top');
    
    if (scrollBtn) {
        function toggleScrollButton() {
            if (window.pageYOffset > 300) {
                scrollBtn.classList.add('visible');
            } else {
                scrollBtn.classList.remove('visible');
            }
        }
        
        toggleScrollButton();
        
        scrollBtn.addEventListener('click', function(e) {
            e.preventDefault();
            window.scrollTo({ top: 0, behavior: 'smooth' });
        });
        
        window.addEventListener('scroll', toggleScrollButton);
    }
});


// ===== FAQ ACCORDION =====
document.addEventListener('DOMContentLoaded', function() {
    const details = document.querySelectorAll('details');
    
    details.forEach(function(detail) {
        detail.addEventListener('toggle', function() {
            if (detail.open) {
                details.forEach(function(other) {
                    if (other !== detail && other.open) {
                        other.open = false;
                    }
                });
            }
        });
    });
});


// ===== SCROLL ANIMATION =====
document.addEventListener('DOMContentLoaded', function() {
    const observerOptions = { 
        threshold: 0.1, 
        rootMargin: '0px 0px -50px 0px' 
    };
    
    const observer = new IntersectionObserver(function(entries) {
        entries.forEach(function(entry) {
            if (entry.isIntersecting) {
                entry.target.style.opacity = '1';
                entry.target.style.transform = 'translateY(0)';
            }
        });
    }, observerOptions);

    const animatedElements = document.querySelectorAll('.card-animate');
    
    animatedElements.forEach(function(el) {
        el.style.opacity = '0';
        el.style.transform = 'translateY(30px)';
        el.style.transition = 'opacity 0.6s ease, transform 0.6s ease';
        observer.observe(el);
    });
});


// ===== DROPDOWN SERVICES =====
document.addEventListener('DOMContentLoaded', function() {
    const serviceItems = document.querySelectorAll('.service-item');
    let allOpen = false;
    
    serviceItems.forEach(function(item) {
        const icon = item.querySelector('.service-icon');
        const dropdownContent = item.querySelector('.dropdown-content');
        
        if (icon && dropdownContent) {
            icon.addEventListener('click', function(e) {
                e.stopPropagation();
                allOpen = !allOpen;
                
                serviceItems.forEach(function(otherItem) {
                    const otherIcon = otherItem.querySelector('.service-icon');
                    const otherDropdown = otherItem.querySelector('.dropdown-content');
                    
                    if (allOpen) {
                        otherIcon.classList.add('active');
                        otherDropdown.classList.add('active');
                    } else {
                        otherIcon.classList.remove('active');
                        otherDropdown.classList.remove('active');
                    }
                });
            });
        }
    });
});


// ===== MOBILE ACCORDION (OFFRES TABLE) =====
function createMobileAccordion() {
    const table = document.querySelector('.offres-table');
    if (!table) return;

    const isMobile = window.innerWidth <= 768;
    
    if (isMobile && !document.querySelector('.mobile-accordion-container')) {
        const tbody = table.querySelector('tbody');
        const rows = Array.from(tbody.querySelectorAll('tr'));
        
        const offers = [
            { 
                name: 'Starter', 
                subtitle: 'Pour débuter', 
                price: '1 800 €', 
                popular: false, 
                columnIndex: 1 
            },
            { 
                name: 'Business', 
                subtitle: 'Le plus choisi', 
                price: '2 500 €', 
                popular: true, 
                columnIndex: 2 
            },
            { 
                name: 'Premium', 
                subtitle: 'Performance maximale', 
                price: '3 500 €', 
                popular: false, 
                columnIndex: 3 
            }
        ];
        
        const accordionContainer = document.createElement('div');
        accordionContainer.className = 'mobile-accordion-container';
        
        offers.forEach(function(offer) {
            const accordion = document.createElement('div');
            accordion.className = 'accordion-item';
            
            if (offer.popular) {
                accordion.classList.add('popular');
            }
            
            if (offer.popular) {
                const badge = document.createElement('div');
                badge.className = 'popular-badge';
                badge.innerHTML = '<i class="fas fa-star"></i> PLUS POPULAIRE';
                accordion.appendChild(badge);
            }
            
            const header = document.createElement('div');
            header.className = 'accordion-header';
            header.innerHTML = `
                <div class="accordion-header-content">
                    <div class="accordion-title">
                        <h3>${offer.name}</h3>
                        <span class="accordion-subtitle">${offer.subtitle}</span>
                    </div>
                    <div class="accordion-price">
                        ${offer.price}
                        <span>/mois HT</span>
                    </div>
                </div>
                <i class="fas fa-chevron-down accordion-icon"></i>
            `;
            
            const content = document.createElement('div');
            content.className = 'accordion-content';
            let contentHTML = '<div class="accordion-details">';
            
            rows.forEach(function(row) {
                const cells = row.querySelectorAll('td');
                if (cells.length > 0) {
                    const label = cells[0].innerHTML;
                    const value = cells[offer.columnIndex].innerHTML;
                    contentHTML += `
                        <div class="detail-row">
                            <div class="detail-label">${label}</div>
                            <div class="detail-value">${value}</div>
                        </div>
                    `;
                }
            });
            
            contentHTML += '</div>';
            contentHTML += `
                <div class="accordion-cta">
                    <a href="contact.html" class="btn-choose-offer">
                        <span>Choisir ${offer.name}</span>
                        <i class="fas fa-arrow-right"></i>
                    </a>
                </div>
            `;
            content.innerHTML = contentHTML;
            
            accordion.appendChild(header);
            accordion.appendChild(content);
            
            header.addEventListener('click', function() {
                accordion.classList.toggle('active');
            });
            
            accordionContainer.appendChild(accordion);
        });
        
        table.parentNode.insertBefore(accordionContainer, table);
        
    } else if (!isMobile) {
        const accordion = document.querySelector('.mobile-accordion-container');
        if (accordion) {
            accordion.remove();
        }
    }
}

document.addEventListener('DOMContentLoaded', createMobileAccordion);

let resizeTimer;
window.addEventListener('resize', function() {
    clearTimeout(resizeTimer);
    resizeTimer = setTimeout(createMobileAccordion, 250);
});