//
//  FormularioController.m
//  ContatosIP67
//
//  Created by ios2534 on 14/03/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "FormularioController.h"

@implementation FormularioController

@synthesize campoNome, campoEmail;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

#pragma mark - View lifecycle

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
    UIBarButtonItem *voltar = [[UIBarButtonItem alloc]
                               initWithBarButtonSystemItem:UIBarButtonSystemItemCancel
                               target:self
                               action:@selector(voltar)
                               ];
    
    UIBarButtonItem *adicionar = [[UIBarButtonItem alloc]
                               initWithBarButtonSystemItem:UIBarButtonSystemItemDone
                               target:self
                               action:@selector(adicionar)
                               ];
    
    self.navigationItem.leftBarButtonItem = voltar;
    self.navigationItem.RightBarButtonItem = adicionar;
}

-(void) voltar
{
    [self dismissModalViewControllerAnimated:YES];
}

-(void) adicionar
{
    [self dismissModalViewControllerAnimated:YES];
}


- (void)viewDidUnload
{
    [self setCampoNome:nil];
    [self setCampoNome:nil];
    [self setCampoEmail:nil];
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

@end
