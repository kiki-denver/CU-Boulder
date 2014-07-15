#include "poly.h"
#include <cassert>
using namespace std;


namespace main_savitch_5
{

	polynomial::polynomial(double c, unsigned int e) //DONE
	{
	/* polynomial class member variables:
	 * EPSILON			=	
	 * @head_ptr 		= 	always points to a node with an exponent of 0, its back()
	 * 						is always equal to NULL
	 * @tail_ptr			= 	points to the last node in the list (or head_ptr if it is
	 * 						a list of one). its fore() is always equal to NULL
	 * @recent_ptr 		=	points to the most recently used node in the polynomial.
	 * @current_degree	=	is always the highest exponent of the polynomial. Is 0
	 *						if the polynomial only has 1 node.
	 */
		// store machine epsilon
		EPSILON = std::numeric_limits<double>::epsilon();

		if (c==0.0 || e==0){
			head_ptr = new polynode(c);
			tail_ptr = head_ptr;
			recent_ptr = head_ptr;
			current_degree = 0;
		}
		else{
			head_ptr = new polynode ();
			tail_ptr = new polynode(c, e);
			recent_ptr = tail_ptr;
			current_degree = e;
			head_ptr->set_fore(tail_ptr);
			tail_ptr->set_back(head_ptr);
		}
    }

    polynomial& polynomial::operator=(const polynomial& source) //DONE
    {
		// store machine epsilon
		EPSILON = std::numeric_limits<double>::epsilon();

		if (this == &source){
			return *this;
		}
		
		if (this->head_ptr != NULL){
			this->clear();	
		}
		delete this->head_ptr;
		this->head_ptr = NULL;
		this->tail_ptr = NULL;
		this->recent_ptr = NULL;
		this->current_degree = 0;
		
		if (source.head_ptr != NULL){
			this->head_ptr = new polynode(source.head_ptr->coef(),
			source.head_ptr->exponent());
			this->tail_ptr = this->head_ptr;
			this->recent_ptr = this->head_ptr;
			this->current_degree = 0;
		//we don't want to replicate the head_ptr we just made above,
		//hence next_term(0)
		for (unsigned int exp = source.next_term(0); exp != 0;
		exp = source.next_term(exp)){
			this->assign_coef(source.coefficient(exp), exp);
			}
		//don't need this if assign_coef takes care of it...
		this->current_degree = source.degree();
		}
		return *this;
	}
	
	bool polynomial::operator==(const polynomial& p1) const //DONE
	{
		if (this == &p1){
			return true;
		}
	    assert ((*this).head_ptr != NULL && p1.head_ptr != NULL);
		p1.set_recent(0);
	    (*this).set_recent(0);
	    if ((*this).degree() != p1.degree()){
			return false;
		}
		while (recent_ptr->fore() != NULL && p1.recent_ptr->fore() != NULL){
			if (recent_ptr->exponent() != p1.recent_ptr->exponent()){
				return false;
			}
			else if (recent_ptr->coef() != p1.recent_ptr->coef()){
				return false;
			}
			else{
			recent_ptr = recent_ptr->fore();
			p1.recent_ptr = p1.recent_ptr->fore();
			} 
		}
		if (recent_ptr->fore() != NULL || p1.recent_ptr->fore() != NULL){
			return false;
		}
		else{
			return true;
		}
	}
	
	bool polynomial::operator!=(const polynomial& p1) const //DONE
	{
		return !(operator==(p1));
	}

			
	
    polynomial::polynomial(const polynomial& source) //DONE
    {
		// store machine epsilon
		EPSILON = std::numeric_limits<double>::epsilon();

		if (source.head_ptr != NULL){
			head_ptr = new polynode(source.head_ptr->coef(),
			source.head_ptr->exponent());
			tail_ptr = head_ptr;
			recent_ptr = head_ptr;
			current_degree = 0;		
		}
		*this = source;
	}

    polynomial::~polynomial() //DONE
    {
		clear();
		delete head_ptr;
		head_ptr = NULL;
		tail_ptr = NULL;
		recent_ptr = NULL;
    }

    void polynomial::clear() //DONE
    {
		polynode* cursor;
		//while there is a polynode after the head_ptr
		while (head_ptr->fore() != NULL){
			cursor = head_ptr->fore();
			delete head_ptr;
			head_ptr = cursor;
		}
		
		recent_ptr = head_ptr;
		head_ptr->set_coef(0.0);
		head_ptr->set_exponent(0);
		tail_ptr = head_ptr;
		current_degree = 0;
		head_ptr->set_back(NULL);
		//probably not necessary
		head_ptr->set_fore(NULL);
    }
    
    double polynomial::coefficient(unsigned int e) const //DONE
    {
		set_recent(e);
		if (recent_ptr->exponent() == e){
			return recent_ptr->coef();
		}
		else{
			return 0.0;
		}
	}
		//~ if (e == 0){
			//~ recent_ptr = head_ptr;
			//~ return recent_ptr->coef();
		//~ }
		//~ else{
			//~ polynode* cursor = head_ptr;
			//~ //get to the polynode right before e
			//~ while (cursor->fore() != NULL && cursor->fore()->exponent() < e){
				//~ cursor = cursor->fore();
			//~ }
			//~ //if e is not actually an exponent in the polynomial
			//~ if (cursor->fore() == NULL || cursor->fore()->exponent() > e){
				//~ rtn = 0.0;
			//~ }
			//~ //else, move forward one, this should be e.
			//~ else{
				//~ recent_ptr = cursor;
				//~ rtn = recent_ptr->fore()->coef();
//~ 
			//~ }
			//~ delete cursor;
			//~ cursor = NULL;
		//~ }
		//~ return rtn;
    

    void polynomial::add_to_coef(double amount, unsigned int e) //DONE
    {
		set_recent(e);
		//the node exists
		double c = 0.0;
		if (recent_ptr->exponent() == e){
			c = recent_ptr->coef() + amount;
		}
		//it doesn't so we're adding to zero
		else{
			c = amount;
		}
		assign_coef(c, e);			
	}

    void polynomial::assign_coef(double c, unsigned int e) //DONE
    {
		set_recent(e);
		//cout << recent_ptr->coef() << "x^" << recent_ptr->exponent() << endl;
		//changing an existing node		
		if (recent_ptr->exponent() == e && fabs(c) >= EPSILON){
			recent_ptr->set_coef(c);
		}
		else if (recent_ptr->exponent() == e && fabs(c) < EPSILON){
			//if it's the head_ptr I don't want to delete it
			if (recent_ptr ==  head_ptr){
//				cout << "it's the head" << endl;
				head_ptr->set_coef(0);
			}
			//if not. BYE-BYE
			else{
				polynode* temp_ptr = recent_ptr;
				//skip from the one before the recent_ptr to the one after
				temp_ptr->back()->set_fore(temp_ptr->fore());
				//don't fall off the end
				if (temp_ptr->fore() != NULL){				
					temp_ptr->fore()->set_back(temp_ptr->back());
				}
				//if recent is the tail do this
				else{
					tail_ptr = temp_ptr->back();
					current_degree = tail_ptr->exponent();
				}
				delete temp_ptr;
			}
			recent_ptr = head_ptr;
		}			
		//not adding anything
		else if (recent_ptr->exponent() != e && fabs(c) < EPSILON){
			return;
		}
		//adding a node at the end of the list
		else if (recent_ptr->fore() == NULL && fabs(c) >= EPSILON){
			polynode* prev_tail = tail_ptr;
			tail_ptr = new polynode(c, e, NULL, prev_tail);
			prev_tail->set_fore(tail_ptr);
			current_degree = e;
			recent_ptr = tail_ptr;						
		}
		//adding a node at the middle of the list
		else{
			polynode* p = new polynode(c, e, recent_ptr->fore(), recent_ptr);
			recent_ptr->fore()->set_back(p);
			recent_ptr->set_fore(p);
			set_recent(e);
			}
	}		
		//cout << "RECENT: " << recent_ptr->coef() << "x^" << recent_ptr->exponent() << endl;		
    
    		//~ set_recent(e);
		//~ //cout << recent_ptr->coef() << "x^" << recent_ptr->exponent() << endl;
		//~ if (e == 0){
			//~ head_ptr->set_coef(c);
		//~ }
		//~ else{
			//~ //deleting an existing node this gives seg. fault 
			//~ if (recent_ptr->exponent() == e && fabs(c) < EPSILON){
				//~ polynode* temp_ptr = recent_ptr;
				//~ //don't fall off the front
				//~ while (temp_ptr->back() != NULL){
					//~ temp_ptr->back()->set_fore(temp_ptr->fore());
				//~ }
				//~ //don't fall off the back
				//~ while (temp_ptr->fore() != NULL){				
					//~ temp_ptr->fore()->set_back(temp_ptr);
				//~ }
				//~ //recent_ptr = tail_ptr;					
				//~ if (current_degree == e){
					//~ current_degree = tail_ptr->exponent();
				//~ }
				//~ delete temp_ptr;
			//~ }			
			//~ //changing an existing node
			//~ else if (recent_ptr->exponent() == e && fabs(c) >= EPSILON){
				//~ recent_ptr->set_coef(c);
			//~ }
			//~ //not adding anything
			//~ else if (recent_ptr->exponent() != e && fabs(c) < EPSILON){
				//~ return;
			//~ }
			//~ //adding a node at the end of the list
			//~ else if (recent_ptr->fore() == NULL && fabs(c) >= EPSILON){
				//~ polynode* prev_tail = tail_ptr;
				//~ tail_ptr = new polynode(c, e, NULL, prev_tail);
				//~ prev_tail->set_fore(tail_ptr);
				//~ current_degree = e;
				//~ //recent_ptr = tail_ptr;						
			//~ }
			//~ //adding a node at the middle of the list
			//~ else{
				//~ polynode* p = new polynode(c, e, recent_ptr->fore(), recent_ptr);
				//~ recent_ptr->fore()->set_back(p);
				//~ recent_ptr->set_fore(p);
				//~ //set_recent(e);
			//~ }
		//~ }
		//~ set_recent(e);
		//~ //cout << "RECENT: " << recent_ptr->coef() <

    unsigned int polynomial::next_term(unsigned int e) const //DONE
    {
			//we want to jump out and not change anything if a next_term is not possible)
			if (e >= current_degree){
				return 0;
			}
			set_recent(e);
			//this is a CYA...theoretically it should never get here...
			if (recent_ptr->fore() == NULL){
				return 0;
			}
			else{
				return recent_ptr->fore()->exponent();
			}
    }

	unsigned int polynomial::previous_term(unsigned int e) const //DONE
    {
		if (e <= 0){
			return UINT_MAX;
		}
		else{
			set_recent(e-1);
		}
		//if the head node is empty...
		if (recent_ptr->back() == NULL && recent_ptr->coef() == 0){
			return UINT_MAX;
		}
		return recent_ptr->exponent();
	}
    
    void polynomial::set_recent(unsigned int e) const //DONE
    {
		if (e==0){
			recent_ptr = head_ptr;
		//cout << "1: " << recent_ptr->coef() << "x^" << recent_ptr->exponent() << endl;
		}
		else if (recent_ptr->exponent() != e){

			if (e >= current_degree){
				recent_ptr = tail_ptr;
		//cout << "2: " << recent_ptr->coef() << "x^" << recent_ptr->exponent() << endl;				
			}
			else if (recent_ptr->exponent() < e){
				while (recent_ptr->fore() != NULL //don't fall off the end//
				&& recent_ptr->fore()->exponent() <= e){
					recent_ptr = recent_ptr->fore();
		//cout << "3: " << recent_ptr->coef() << "x^" << recent_ptr->exponent() << endl;				
				}
			}
	
			else{
				recent_ptr = head_ptr;
				while (recent_ptr->fore() != NULL //don't fall off the end//
				&& recent_ptr->fore()->exponent() <= e){
					recent_ptr = recent_ptr->fore();
		//cout << "4: " << recent_ptr->coef() << "x^" << recent_ptr->exponent() << endl;				
				}
			}
		}
		
	}
    
    double polynomial::eval(double x) const //DONE
    {
//     POSTCONDITION: The return value is the value of this polynomial with the
//     given value for the variable x.		
		//~ set_recent(0);
		//~ double factor = x;
		//~ //takes care of the head node aka the x^0 case
		//~ double total = recent_ptr->coef();
		//~ //takes care of the x^1 case
		//~ if (recent_ptr->fore()->exponent() == 1){
			//~ recent_ptr = recent_ptr->fore();
			//~ total = total + (recent_ptr->coef() * x);
		//~ }
		//~ while (recent_ptr->fore() != NULL){
			//~ recent_ptr = recent_ptr->fore();
			//~ for (unsigned int i = 1; i<recent_ptr->exponent(); ++i){
				//~ factor = factor * x;
			//~ }
			//~ total = total + recent_ptr->coef() * factor;
			//~ factor = x;
		//~ }
		//~ return total;
		
		double total = coefficient(0);
		unsigned int expo = next_term(0);
		
		while (expo != 0)
		{
			total += coefficient(expo)*pow(x, double(expo));
			expo = next_term(expo);	
		}
		return total;		
    }

    polynomial polynomial::derivative() const //DONE
    {
	//	cout << "1" << endl;		
		polynomial p_prime;
	//	cout << "2" << endl;
		polynode* cursor = head_ptr;
	//	cout << "3" << endl;		
		if (cursor->fore() == NULL){
			return p_prime;
	//	cout << "4" << endl;			
		}
		else{
	//	cout << "5" << endl;			
			while (cursor->fore() != NULL){
	//	cout << "6" << endl;				
				cursor = cursor->fore();
	//	cout << "7" << endl;				
				p_prime.assign_coef((cursor->exponent() * cursor->coef()),
					(cursor->exponent() - 1));
	//	cout << "8" << endl;					
	//			cout << (cursor->exponent() * cursor->coef()) << ", " << 
	//				(cursor->exponent() - 1) << endl;
				}
	//	cout << "9" << endl;								
			return p_prime;
	//	cout << "10" << endl;			
		}
    }
	
	double polynomial::definite_integral(double a, double b) const //TODO
	{
		assert(b >= a);
		polynomial p;
		set_recent(next_term(0));
		assert (recent_ptr->exponent() != 0);
		do{
			p.assign_coef((recent_ptr->coef()/(recent_ptr->exponent()+1)),
				(recent_ptr->exponent()+1));
			set_recent(next_term(recent_ptr->exponent()));
		} while(recent_ptr->fore() != NULL);
		double def_int = p.eval(b)- p.eval(a);	
		return def_int;
	}
    
    polynomial operator+(const polynomial& p1, const polynomial& p2) //DONE
    {
		unsigned int maxnodes = 0.0;
		if (p1.degree() >= p2.degree()){
			maxnodes = p1.degree() + 1;
			}
		else{
			maxnodes = p2.degree() + 1;
		}
		double* factors = new double[maxnodes];
		for (unsigned int i = 0; i < maxnodes; ++i){
			factors[i] = 0.0;
		}
		factors[0] = p1.coefficient(0) + p2.coefficient(0);
		
		for (unsigned int exp = p1.next_term(0); exp != 0; exp = p1.next_term(exp)){
			factors[exp] = factors[exp] + p1.coefficient(exp);
		}
		for (unsigned int exp = p2.next_term(0); exp != 0; exp = p2.next_term(exp)){
			factors[exp] = factors[exp] + p2.coefficient(exp);
		}
				
		polynomial result;
		
		for (unsigned int i = 0; i < maxnodes; ++i){
			result.assign_coef (factors[i], i);
		}
		
		delete [] factors;
		factors = NULL; 		
		return result;
    }
    
    polynomial operator-(const polynomial& p1, const polynomial& p2) //DONE
    {
		unsigned int maxnodes = 0.0;
		if (p1.degree() >= p2.degree()){
			maxnodes = p1.degree() + 1;
			}
		else{
			maxnodes = p2.degree() + 1;
		}
		
		polynomial result;		
		for (unsigned int exp = 0; exp < maxnodes; exp++){
			result.assign_coef (p1.coefficient(exp) - p2.coefficient(exp), exp);
		}
		
		return result;
    }
    
    polynomial operator*(const polynomial& p1, const polynomial& p2) //TODO
    {		
		polynomial result;
		unsigned int exp1 = 0;
		unsigned int exp2 = 0;
		do{
			do{
				result.add_to_coef((p1.coefficient(exp1) * p2.coefficient(exp2)),
				(exp1 +  exp2));
				exp2 = p2.next_term(exp2);
			} while (exp2 != 0);
			
			exp2 = 0;			
			exp1 = p1.next_term(exp1);
		} while (exp1 != 0);
			
	return result;
    }

    ostream& operator << (ostream& out, const polynomial& p) //DONE
    {
		unsigned int exp = 0;
		
		do{ //will at least go through the loop one time even if at first
			//the while loop is false
			if (exp == 0 && p.coefficient(exp) == 0){
			}
			else{
				if (exp == 0 && p.coefficient(exp) != 0){
					out << p.coefficient(exp);
				}
				else if (exp !=0 && p.coefficient(exp) != 0){
					out << p.coefficient(exp) << "x^" << exp;
				}
				else if (exp != 0 && p.coefficient(exp) == 0){
					out << "x^" << exp;
				}
				if (p.next_term(exp) != 0){
				 out << " + ";
				}
			}
			exp = p.next_term(exp);
		} while(exp != 0);
	
		return out;
    }
    
    void polynomial::find_root(
	double& answer,
	bool& success,
	unsigned int& iterations,
	double guess,
	unsigned int maximum_iterations,
	double epsilon) const
    {
	}
}
