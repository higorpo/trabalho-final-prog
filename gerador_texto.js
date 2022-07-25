const palavras = ['ola', 'homem', 'como', 'porque', 'onde', 'voce', 'bem', 'mal',
    'vencemos', 'perdemos', 'vida', 'amor', 'paz', 'derrota', 'saude', 'saudade',
    'liberdade', 'crescimento', 'mulher', 'passaro', 'carro', 'moto', 'barco', 'uber',
    'facebook']

let texto = '';

const size = 9000;

for (let i = 0; i < size; i++) {
    texto += palavras[Math.floor(Math.random() * palavras.length)];
}

console.log(texto.slice(0, size - 1))