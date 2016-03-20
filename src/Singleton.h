#pragma once

template<typename T> class cSingleton // plantilla singleton toma un valor "T"
{
	public:
		static T& Get()	// declara un metodo estatico "Get" con retorno T
		{
			static T lInstance; // el cual define una instancia estatica del tipo "T"(la clase)
			return lInstance; // y luego devuelve esa instancia unica de una sola clase/objeto para acceso publico
		}
}; // directamente al declarar una determinada clase se usa una sola véz y puede accederse from everywhere con Get() 